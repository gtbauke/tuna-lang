#include "token.h"

token new_token(token_kind kind, const char* lexeme, size_t length) {
  return (token) {
    .kind = kind,
    .lexeme = lexeme,
    .length = length
  };
}
