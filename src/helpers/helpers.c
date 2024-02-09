#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

InputBuffer *new_input_buffer() {
  InputBuffer *input_buffer = malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("Enter query > "); }

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
