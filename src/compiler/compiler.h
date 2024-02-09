#pragma once
#include <string.h>

#include "../helpers/helpers.h"

typedef enum { METACOMMAND_SUCCESS, METACOMMAND_FAILURE } MetaCommandResult;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_SYNTAX_ERROR,
  PREPARE_FAILURE
} PreparationResult;

void read_query(InputBuffer *input_buffer);

PreparationResult prepare_statement(InputBuffer *input_buffer,
                                    Statement *statement);

MetaCommandResult execute_meta_commands(InputBuffer *input_buffer,
                                        Table *table);
