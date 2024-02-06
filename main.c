#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

typedef enum { METACOMMAND_SUCCESS, METACOMMAND_FAILURE } MetaCommandResult;
typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;
typedef enum { PREPARE_SUCCESS, PREPARE_FAILURE } PreparationResult;

typedef struct {
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef struct {
  StatementType type;
} Statement;

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

void close_input_buffer(InputBuffer *input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

MetaCommandResult execute_meta_commands(InputBuffer *input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    exit(EXIT_SUCCESS);
  } else {
    return METACOMMAND_FAILURE;
  }
}

PreparationResult prepare_statement(InputBuffer *input_buffer,
                                    Statement *statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  } else if (strncmp(input_buffer->buffer, "select", 6) == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_FAILURE;
}

void execute_prepared_statement(Statement *statement) {
  switch (statement->type) {
  case STATEMENT_INSERT:
    printf("Performing an insert operation\n");
    break;
  case STATEMENT_SELECT:
    printf("Performing a select operation\n");
    break;
  }
}

int main() {
  InputBuffer *input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_query(input_buffer);

    if (input_buffer->buffer[0] == '.') {
      switch (execute_meta_commands(input_buffer)) {
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
      execute_prepared_statement(&statement);
      break;
    case PREPARE_FAILURE:
      printf("Unable to process SQL command '%s'\n", input_buffer->buffer);
      break;
    }
  }
}
