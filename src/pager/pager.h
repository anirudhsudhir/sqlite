#pragma once

#include <fcntl.h>
#include <stdint.h>

#define PAGE_SIZE 4096
#define ROW_SIZE sizeof(Row)

#define MAX_PAGES_PER_TABLE 100

typedef struct {
  int file_descriptor;
  u_int32_t file_length;

  void *pages[MAX_PAGES_PER_TABLE];
} Pager;

Pager *new_pager(char *filepath);

void *get_page(Pager *pager, u_int32_t page_num);

void flush_page(Pager *pager, u_int32_t page_num, off_t page_size);
