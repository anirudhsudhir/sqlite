#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"

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

PreparationResult prepare_insert_statement(InputBuffer *input_buffer,
                                           Statement *statement) {
  strtok(input_buffer->buffer, " ");
  char *id = strtok(NULL, " ");
  char *username = strtok(NULL, " ");
  char *email = strtok(NULL, " ");
  if (id == NULL || username == NULL || email == NULL) {
    return PREPARE_SYNTAX_ERROR;
  }
  if ((strlen(username) > MAX_USERNAME_LENGTH) ||
      (strlen(email) > MAX_EMAIL_LENGTH)) {
    return PREPARE_STRING_LENGTH_EXCEEDED;
  }
  int id_num = atoi(id);
  statement->row.ID = id_num;
  if (id_num < 0) {
    return PREPARE_ID_NEGATIVE;
  }

  strcpy(statement->row.USERNAME, username);
  strcpy(statement->row.EMAIL, email);
  return PREPARE_SUCCESS;
}

PreparationResult prepare_statement(InputBuffer *input_buffer,
                                    Statement *statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return prepare_insert_statement(input_buffer, statement);
  } else if (strncmp(input_buffer->buffer, "select", 6) == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_FAILURE;
}

MetaCommandResult execute_meta_commands(InputBuffer *input_buffer,
                                        Table *table) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    db_close(table);

    exit(EXIT_SUCCESS);
  } else {
    return METACOMMAND_FAILURE;
  }
}
