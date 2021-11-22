#ifndef tuna_lexer_h
#define tuna_lexer_h

#include <stdbool.h>
#include "token.h"

typedef struct {
  const char* source;
  size_t source_length;

  unsigned int start;
  unsigned int current;

  int line;
  int column;
  const char* file_path;
} tuna_lexer;

tuna_lexer* new_lexer(const char* source, const char* file_path);
void free_lexer(tuna_lexer* lexer);

token next_token(tuna_lexer* lexer);
bool is_lexer_at_end(tuna_lexer* lexer);

#endif
