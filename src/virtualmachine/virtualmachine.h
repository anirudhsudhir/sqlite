#pragma once

#include <stdlib.h>

#include "../helpers/helpers.h"

#define PAGE_SIZE 4096
#define ROW_SIZE sizeof(Row)
#define MAX_ROWS_PER_PAGE (PAGE_SIZE / ROW_SIZE)
#define MAX_ROWS_PER_TABLE (MAX_ROWS_PER_PAGE * MAX_PAGES_PER_TABLE)

typedef enum { EXECUTION_SUCCESS, EXECUTION_FAILURE } ExecutionResult;

#define sizeof_type(struct, attribute) sizeof(((struct *)0)->attribute)

int *compute_offset(Table *table, u_int32_t row_num);

void serialize_rows(Row *source, void *destination);

void deserialize_rows(void *source, Row *destination);

void print_row(Row *row);

ExecutionResult perform_insert_operation(Table *table, Row *row_to_insert);

ExecutionResult perform_select_operation(Table *table);

ExecutionResult execute_prepared_statement(Statement *statement, Table *table);
