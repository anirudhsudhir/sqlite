#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pager.h"

Pager *new_pager(char *filepath) {
  Pager *pager = (Pager *)malloc(sizeof(Pager));
  int fd = open(filepath, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
  if (fd == -1) {
    printf("Error: Unable to open db file\n");
    exit(EXIT_FAILURE);
  }

  int fl = lseek(fd, 0, SEEK_END);
  if (fl == -1) {
    printf("Error: Error seeking db file\n");
    exit(EXIT_FAILURE);
  }

  pager->file_descriptor = fd;
  pager->file_length = fl;

  for (int i = 0; i < MAX_PAGES_PER_TABLE; i++) {
    pager->pages[i] = NULL;
  }

  return pager;
}

void *get_page(Pager *pager, u_int32_t page_num) {
  if (page_num >= MAX_PAGES_PER_TABLE) {
    printf("Error: Table contains maximum pages");
    exit(EXIT_FAILURE);
  }

  void *page = pager->pages[page_num];
  u_int32_t total_pages = pager->file_length % PAGE_SIZE;

  if (page == NULL) {
    page = malloc(PAGE_SIZE);
    if (pager->file_length % PAGE_SIZE) {
      total_pages += 1;
    }

    if (page_num <= total_pages) {
      off_t seek =
          lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
      if (seek == -1) {
        printf("Error: Error seeking db file\n");
        exit(EXIT_FAILURE);
      }
      ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);
      if (bytes_read == -1) {
        printf("Error: Error reading db file\n");
        exit(EXIT_FAILURE);
      }
    }

    pager->pages[page_num] = page;
  }

  return pager->pages[page_num];
}

void flush_page(Pager *pager, u_int32_t page_num, off_t page_size) {
  void *page = pager->pages[page_num];

  if (page == NULL) {
    printf("Error: Flushing empty page to disk\n");
    exit(EXIT_FAILURE);
  }

  off_t seek = lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
  if (seek == -1) {
    printf("Error: Error seeking db file\n");
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_written = write(pager->file_descriptor, page, page_size);
  if (bytes_written == -1) {
    printf("Error: Error writing db file\n");
    exit(EXIT_FAILURE);
  }
}
