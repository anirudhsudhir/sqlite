#pragma once
#include <string.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define USERNAME_MAX_LENGTH 32
#define EMAIL_MAX_LENGTH 255
#define MAX_PAGES_PER_TABLE 100

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef struct {
  u_int32_t total_rows;
  void *pages[MAX_PAGES_PER_TABLE];
} Table;

typedef struct {
  u_int32_t ID;
  char USERNAME[USERNAME_MAX_LENGTH];
  char EMAIL[EMAIL_MAX_LENGTH];
} Row;

typedef struct {
  StatementType type;
  Row row_to_insert;
} Statement;

InputBuffer *new_input_buffer();

void close_input_buffer(InputBuffer *input_buffer);

Table *new_table();

void free_table(Table *table);

void print_prompt();
