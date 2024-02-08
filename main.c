#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define MAX_PAGES_PER_TABLE 100
#define PAGE_SIZE 4096
#define ROW_SIZE sizeof(Row)
#define MAX_ROWS_PER_PAGE (PAGE_SIZE / ROW_SIZE)
#define MAX_ROWS_PER_TABLE (MAX_ROWS_PER_PAGE * MAX_PAGES_PER_TABLE)

#define USERNAME_MAX_LENGTH 32
#define EMAIL_MAX_LENGTH 255

typedef enum { METACOMMAND_SUCCESS, METACOMMAND_FAILURE } MetaCommandResult;
typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;
typedef enum {
  PREPARE_SUCCESS,
  PREPARE_SYNTAX_ERROR,
  PREPARE_FAILURE
} PreparationResult;
typedef enum { EXECUTION_SUCCESS, EXECUTION_FAILURE } ExecutionResult;

typedef struct {
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef struct {
  u_int32_t ID;
  char USERNAME[USERNAME_MAX_LENGTH];
  char EMAIL[EMAIL_MAX_LENGTH];
} Row;

typedef struct {
  StatementType type;
  Row row_to_insert;
} Statement;

typedef struct {
  u_int32_t total_rows;
  void *pages[MAX_PAGES_PER_TABLE];
} Table;

#define sizeof_type(struct, attribute) sizeof(((struct *)0)->attribute)

const int ROW_ID_SIZE = sizeof_type(Row, ID);
const int ROW_USERNAME_SIZE = sizeof_type(Row, USERNAME);
const int ROW_EMAIL_SIZE = sizeof_type(Row, EMAIL);
const int ROW_ID_OFFSET = 0;
const int ROW_USERNAME_OFFSET = ROW_ID_OFFSET + ROW_ID_SIZE;
const int ROW_EMAIL_OFFSET = ROW_USERNAME_OFFSET + ROW_USERNAME_SIZE;

InputBuffer *new_input_buffer() {
  InputBuffer *input_buffer = malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("Enter query > "); }

void read_query(InputBuffer *input_buffer) {
  int input_length =
      getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (input_length <= 0) {
    printf("Unable to read query\n");
    exit(EXIT_FAILURE);
  }

  input_buffer->input_length = input_length - 1;
  input_buffer->buffer[input_length - 1] = 0;
}

Table *new_table() {
  Table *table = (Table *)malloc(sizeof(Table));
  table->total_rows = 0;
  for (int i = 0; i < MAX_PAGES_PER_TABLE; i++) {
    table->pages[i] = NULL;
  }
  return table;
}

void close_input_buffer(InputBuffer *input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

void free_table(Table *table) {
  for (int i = 0; i < MAX_PAGES_PER_TABLE; i++) {
    if (table->pages[i] != NULL) {
      free(table->pages[i]);
    }
  }
  free(table);
}

int *compute_offset(Table *table, u_int32_t row_num) {
  int page_num = row_num / MAX_ROWS_PER_PAGE;
  void *page = table->pages[page_num];
  if (page == NULL) {
    page = table->pages[page_num] = malloc(PAGE_SIZE);
  }
  u_int32_t byte_offset = (row_num % MAX_ROWS_PER_PAGE) * ROW_SIZE;

  return page + byte_offset;
}

void serialize_rows(Row *source, void *destination) {
  memcpy(destination + ROW_ID_OFFSET, &(source->ID), ROW_ID_SIZE);
  memcpy(destination + ROW_USERNAME_OFFSET, source->USERNAME,
         ROW_USERNAME_SIZE);
  memcpy(destination + ROW_EMAIL_OFFSET, source->EMAIL, ROW_EMAIL_SIZE);
}

void deserialize_rows(void *source, Row *destination) {
  memcpy(&(destination->ID), source + ROW_ID_OFFSET, ROW_ID_SIZE);
  memcpy(destination->USERNAME, source + ROW_USERNAME_OFFSET,
         ROW_USERNAME_SIZE);
  memcpy(destination->EMAIL, source + ROW_EMAIL_OFFSET, ROW_EMAIL_SIZE);
}

void print_row(Row *row) {
  printf("(%d,%s,%s)\n", row->ID, row->USERNAME, row->EMAIL);
}

MetaCommandResult execute_meta_commands(InputBuffer *input_buffer,
                                        Table *table) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    free_table(table);
    exit(EXIT_SUCCESS);
  } else {
    return METACOMMAND_FAILURE;
  }
}

PreparationResult prepare_statement(InputBuffer *input_buffer,
                                    Statement *statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    int args_read = sscanf(
        input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.ID),
        statement->row_to_insert.USERNAME, statement->row_to_insert.EMAIL);

    if (args_read < 3) {
      return PREPARE_SYNTAX_ERROR;
    }

    return PREPARE_SUCCESS;
  } else if (strncmp(input_buffer->buffer, "select", 6) == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_FAILURE;
}

ExecutionResult perform_insert_operation(Table *table, Row *row_to_insert) {

  serialize_rows(row_to_insert, compute_offset(table, table->total_rows));
  table->total_rows += 1;

  return EXECUTION_SUCCESS;
}

ExecutionResult perform_select_operation(Table *table) {
  for (u_int32_t i = 0; i < table->total_rows; i++) {
    Row row;
    deserialize_rows(compute_offset(table, i), &row);
    print_row(&row);
  }

  return EXECUTION_SUCCESS;
}

ExecutionResult execute_prepared_statement(Statement *statement, Table *table) {
  switch (statement->type) {
  case STATEMENT_INSERT:
    return perform_insert_operation(table, &(statement->row_to_insert));
  case STATEMENT_SELECT:
    return perform_select_operation(table);
  }
}

int main() {
  InputBuffer *input_buffer = new_input_buffer();
  Table *table = new_table();
  while (true) {
    print_prompt();
    read_query(input_buffer);

    if (input_buffer->buffer[0] == '.') {
      switch (execute_meta_commands(input_buffer, table)) {
      case METACOMMAND_SUCCESS:
        break;
      case METACOMMAND_FAILURE:
        printf("Invalid meta command '%s'\n", input_buffer->buffer);
        continue;
      }
    }

    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
    case PREPARE_SUCCESS:
      break;
    case PREPARE_SYNTAX_ERROR:
      printf("Syntax error in SQL command\n");
      continue;
    case PREPARE_FAILURE:
      printf("Unable to process SQL command '%s'\n", input_buffer->buffer);
      continue;
    }

    switch (execute_prepared_statement(&statement, table)) {
    case EXECUTION_SUCCESS:
      printf("Executed successfully\n");
      break;
    case EXECUTION_FAILURE:
      printf("Execution failed\n");
      break;
    }
  }
}
