#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "ast.h"
#include "printer.h"

static const char* read_file(const char* file_path) {
  FILE* file = fopen(file_path, "rb");
  if (file == NULL) {
    fprintf(stderr, "Could not open file: '%s' - %s\n", file_path, strerror(errno));
    exit(74);
  }

  fseek(file, 0L, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(file_size + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Could not allocate memory for file: '%s' - %s\n", file_path, strerror(errno));
    exit(74);
  }

  size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
  if (bytes_read < file_size) {
    fprintf(stderr, "Could not read file: '%s' - %s\n", file_path, strerror(errno));
    exit(74);
  }

  buffer[bytes_read] = '\0';
  fclose(file);

  return buffer;
}

int main(int agrc, char** argv) {
  const char* file_path = argv[1];
  const char* file_content = read_file(file_path);

  tuna_lexer* lexer = new_lexer(file_content, file_path);
  
  printf("File '%s' content:\n", file_path);
  printf("%s\n\n\n", file_content);

  while (!is_lexer_at_end(lexer)) {
    token token = next_token(lexer);
    printf("(%s:%i:%i) %s - %.*s\n", token.file_path, token.line, token.column, token_kind_as_string(token.kind), token.length, token.lexeme);
  }

  free(file_content);
  free_lexer(lexer);
  return 0;
}
