#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

typedef struct {
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer() {
  InputBuffer *input_buffer = malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("Enter query > "); }

void read_query(InputBuffer *input_buffer) {
  int input_length =
      getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (input_length <= 0) {
    printf("Unable to read query\n");
    exit(EXIT_FAILURE);
  }

  input_buffer->input_length = input_length - 1;
  input_buffer->buffer[input_length - 1] = 0;
}

void close_input_buffer(InputBuffer *input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

int main() {
  InputBuffer *input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_query(input_buffer);

    if (strcmp(input_buffer->buffer, ".exit") == 0) {
      close_input_buffer(input_buffer);
      exit(EXIT_SUCCESS);
    } else {
      printf("Command %s is unrecognized\n", input_buffer->buffer);
    }
  }
}
