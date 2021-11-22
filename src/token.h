#ifndef tuna_token_h
#define tuna_token_h

#include <stdlib.h>

typedef enum {
  TOKEN_OPEN_PAREN,
  TOKEN_CLOSE_PAREN,
  TOKEN_OPEN_BRACKET,
  TOKEN_CLOSE_BRACKET,
  TOKEN_OPEN_CURLY,
  TOKEN_CLOSE_CURLY,
  TOKEN_COMMA,
  TOKEN_COLON,
  TOKEN_DOT,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_PERCENT,
  TOKEN_BANG,
  TOKEN_EQUAL,
  TOKEN_EQUAL_EQUAL,
  TOKEN_BANG_EQUAL,
  TOKEN_LESS,
  TOKEN_LESS_EQUAL,
  TOKEN_GREATER,
  TOKEN_GREATER_EQUAL,

  TOKEN_LET,
  TOKEN_FUN,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_RETURN,
  TOKEN_WHILE,
  TOKEN_FOR,
  TOKEN_BREAK,
  TOKEN_CONTINUE,
  TOKEN_NULL,
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_NOT,

  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_INTEGER,
  TOKEN_FLOAT,
  TOKEN_ERROR,
  TOKEN_EOF,
} token_kind;

typedef struct {
  token_kind kind;

  const char* lexeme;
  size_t length;

  int line;
  int column;
  const char* file_path;
} token;

token new_token(token_kind kind, const char* lexeme, size_t length, int line, int column, const char* file_path);
const char* token_kind_as_string(token_kind kind);

#endif
