#ifndef tuna_lexer_h
#define tuna_lexer_h

#include <stdbool.h>
#include "token.h"

typedef struct {
  const char* source;
  size_t source_length;

  unsigned int start;
  unsigned int current;
} tuna_lexer;

tuna_lexer* new_lexer(const char* source);
void free_lexer(tuna_lexer* lexer);

token next_token(tuna_lexer* lexer);
bool is_lexer_at_end(tuna_lexer* lexer);

#endif
