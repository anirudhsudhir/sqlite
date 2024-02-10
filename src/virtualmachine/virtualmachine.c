#include <stdio.h>
#include <stdlib.h>

#include "virtualmachine.h"

const int ROW_ID_SIZE = sizeof_type(Row, ID);
const int ROW_USERNAME_SIZE = sizeof_type(Row, USERNAME);
const int ROW_EMAIL_SIZE = sizeof_type(Row, EMAIL);
const int ROW_ID_OFFSET = 0;
const int ROW_USERNAME_OFFSET = ROW_ID_OFFSET + ROW_ID_SIZE;
const int ROW_EMAIL_OFFSET = ROW_USERNAME_OFFSET + ROW_USERNAME_SIZE;

ExecutionResult execute_prepared_statement(Statement *statement, Table *table) {
  switch (statement->type) {
  case STATEMENT_INSERT:
    return perform_insert_operation(table, &(statement->row));
  case STATEMENT_SELECT:
    return perform_select_operation(table);
  }
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
  printf("(%d, %s, %s)\n", row->ID, row->USERNAME, row->EMAIL);
}

ExecutionResult perform_insert_operation(Table *table, Row *row_to_insert) {
  if (table->total_rows == MAX_ROWS_PER_TABLE) {
    printf("Error: Table full.\n");
    return EXECUTION_FAILURE;
  }
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
