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

Table *new_table(char *filepath) {
  Table *table = (Table *)malloc(sizeof(Table));

  Pager *pager = new_pager(filepath);

  table->pager = pager;
  table->total_rows = pager->file_length / ROW_SIZE;
  return table;
}

void close_input_buffer(InputBuffer *input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

void free_pager(Pager *pager) {
  for (int i = 0; i < MAX_PAGES_PER_TABLE; i++) {
    if (pager->pages[i] != NULL) {
      free(pager->pages[i]);
    }
  }
  free(pager);
}

void db_close(Table *table) {
  Pager *pager = table->pager;
  u_int32_t num_full_pages = pager->file_length / PAGE_SIZE;

  for (u_int32_t i = 0; i < num_full_pages; i++) {
    if (pager->pages[i] != NULL) {
      flush_page(pager, i, PAGE_SIZE);
      free(pager->pages[i]);
      pager->pages[i] = NULL;
    }
  }

  u_int32_t extra_rows = table->total_rows % MAX_ROWS_PER_TABLE;
  if (extra_rows > 0) {
    if (pager->pages[num_full_pages] != NULL) {
      flush_page(pager, num_full_pages, extra_rows * ROW_SIZE);
      free(pager->pages[num_full_pages]);
      pager->pages[num_full_pages] = NULL;
    }
  }

  free_pager(pager);
  free(table);
}
