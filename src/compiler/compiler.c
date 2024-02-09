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
