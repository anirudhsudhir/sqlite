#pragma once
#include <string.h>

#include "../pager/pager.h"

#define MAX_USERNAME_LENGTH 32
#define MAX_EMAIL_LENGTH 255

#define MAX_ROWS_PER_PAGE (PAGE_SIZE / ROW_SIZE)
#define MAX_ROWS_PER_TABLE (MAX_ROWS_PER_PAGE * MAX_PAGES_PER_TABLE)

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef struct {
  u_int32_t total_rows;
  Pager *pager;
} Table;

typedef struct {
  u_int32_t ID;
  char USERNAME[MAX_USERNAME_LENGTH + 1];
  char EMAIL[MAX_EMAIL_LENGTH + 1];
} Row;

typedef struct {
  StatementType type;
  Row row;
} Statement;

InputBuffer *new_input_buffer();

void close_input_buffer(InputBuffer *input_buffer);

Table *new_table(char *filepath);

void free_table(Table *table);

void print_prompt();

void free_pager(Pager *pager);

void db_close(Table *table);
