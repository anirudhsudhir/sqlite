#include <stdbool.h>
#include <stdio.h>

#include "compiler/compiler.h"
#include "helpers/helpers.h"
#include "virtualmachine/virtualmachine.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Error: Path to db file not provided\n");
    exit(EXIT_FAILURE);
  }

  InputBuffer *input_buffer = new_input_buffer();
  Table *table = new_table(argv[1]);
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
    case PREPARE_STRING_LENGTH_EXCEEDED:
      printf("Invalid string length\n");
      continue;
    case PREPARE_ID_NEGATIVE:
      printf("ID must be positive\n");
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
      break;
    }
  }
}
