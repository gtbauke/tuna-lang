#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

tuna_lexer* new_lexer(const char* source) {
  tuna_lexer* lexer = malloc(sizeof(tuna_lexer));
  lexer->source = source;
  lexer->source_length = strlen(source);

  lexer->start = 0;
  lexer->current = 0;

  return lexer;
}

void free_lexer(tuna_lexer* lexer) {
  free(lexer);
}

bool is_lexer_at_end(tuna_lexer* lexer) {
  return lexer->current >= lexer->source_length;
}

static bool is_digit(char c) {
  return c >= '0' && c <= '9';
}

static bool is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool is_alpha_numeric(char c) {
  return is_digit(c) || is_alpha(c) || c == '-';
}

static char advance(tuna_lexer* lexer) {
  lexer->current++;
  return lexer->source[lexer->current - 1];
}

static char peek(tuna_lexer* lexer) {
  if (is_lexer_at_end(lexer)) return '\0';
  return lexer->source[lexer->current];
}

static char peek_next(tuna_lexer* lexer) {
  if (is_lexer_at_end(lexer)) return '\0';
  return lexer->source[lexer->current + 1];
}

static bool matches(tuna_lexer* lexer, char expected) {
  if (is_lexer_at_end(lexer) || lexer->source[lexer->current] != expected) return false;
  
  advance(lexer);
  return true;
}

static token handle_numbers(tuna_lexer* lexer) {
  token_kind kind = TOKEN_INTEGER;
  while (is_digit(peek(lexer))) advance(lexer);

  if (peek(lexer) == '.' && is_digit(peek_next(lexer))) {
    advance(lexer);
    while (is_digit(peek(lexer))) advance(lexer);

    kind = TOKEN_FLOAT;
  }

  return new_token(kind, &lexer->source[lexer->start], lexer->current - lexer->start);
}

typedef struct {
  const char* lexeme;
  size_t length;
  token_kind kind;
} keyword;

static const keyword keywords[] = {
  { "let", 3, TOKEN_LET },
  { "fun", 3, TOKEN_FUN },
  { "if", 2, TOKEN_IF },
  { "else", 4, TOKEN_ELSE },
  { "true", 4, TOKEN_TRUE },
  { "false", 5, TOKEN_FALSE },
  { "return", 6, TOKEN_RETURN },
  { "while", 5, TOKEN_WHILE },
  { "for", 3, TOKEN_FOR },
  { "break", 5, TOKEN_BREAK },
  { "continue", 8, TOKEN_CONTINUE },
  { "null", 4, TOKEN_NULL },
  { "and", 3, TOKEN_AND },
  { "or", 2, TOKEN_OR },
  { "not", 3, TOKEN_NOT },
};

static token handle_identifiers(tuna_lexer* lexer) {
  while (is_alpha_numeric(peek(lexer))) advance(lexer);

  token_kind kind = TOKEN_IDENTIFIER;
  for (int i = 0; i < sizeof(keywords) / sizeof(keyword); i++) {
    if (lexer->current - lexer->start == keywords[i].length &&
        strncmp(&lexer->source[lexer->start], keywords[i].lexeme, keywords[i].length) == 0) {
      kind = keywords[i].kind;
      break;
    }
  }

  return new_token(kind, &lexer->source[lexer->start], lexer->current - lexer->start);
}

static void skip_whitespace(tuna_lexer* lexer) {
  while (peek(lexer) == ' ' || peek(lexer) == '\t' || peek(lexer) == '\n' || peek(lexer) == '\r') {
    advance(lexer);
  }
}

static void skip_comments(tuna_lexer* lexer) {
  if (peek(lexer) == '/' && peek_next(lexer) == '/') {
    while (peek(lexer) != '\n') advance(lexer);
  } else if (peek(lexer) == '/' && peek_next(lexer) == '*') {
    while (!is_lexer_at_end(lexer) && peek(lexer) != '*' && peek_next(lexer) != '/') advance(lexer);

    if (!is_lexer_at_end(lexer)) {
      fprintf(stderr, "Unterminated comment\n");
      exit(1);
    }

    advance(lexer);
    advance(lexer);
  }
}

static token handle_strings(tuna_lexer* lexer) {
  while (!is_lexer_at_end(lexer) && peek(lexer) != '\'') advance(lexer);

  if (is_lexer_at_end(lexer)) {
    fprintf(stderr, "Unterminated string\n");
    exit(1);
  }

  advance(lexer);
  return new_token(TOKEN_STRING, &lexer->source[lexer->start], (lexer->current - 1) - (lexer->start + 1));
}

token next_token(tuna_lexer* lexer) {
  skip_whitespace(lexer);
  skip_comments(lexer);
  lexer->start = lexer->current;

  char c = advance(lexer);

  if (is_digit(c)) return handle_numbers(lexer);

  switch (c) {
    case '(': return new_token(TOKEN_OPEN_PAREN, &lexer->source[lexer->start], lexer->current - lexer->start);
    case ')': return new_token(TOKEN_CLOSE_PAREN, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '[': return new_token(TOKEN_OPEN_BRACKET, &lexer->source[lexer->start], lexer->current - lexer->start);
    case ']': return new_token(TOKEN_CLOSE_BRACKET, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '{': return new_token(TOKEN_OPEN_CURLY, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '}': return new_token(TOKEN_CLOSE_CURLY, &lexer->source[lexer->start], lexer->current - lexer->start);
    case ',': return new_token(TOKEN_COMMA, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '.': return new_token(TOKEN_DOT, &lexer->source[lexer->start], lexer->current - lexer->start);
    case ':': return new_token(TOKEN_COLON, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '=': return matches(lexer, '=')
      ? new_token(TOKEN_EQUAL_EQUAL, &lexer->source[lexer->start], lexer->current - lexer->start)
      : new_token(TOKEN_EQUAL, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '!': return matches(lexer, '=')
      ? new_token(TOKEN_BANG_EQUAL, &lexer->source[lexer->start], lexer->current - lexer->start)
      : new_token(TOKEN_BANG, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '<': return matches(lexer, '=')
      ? new_token(TOKEN_LESS_EQUAL, &lexer->source[lexer->start], lexer->current - lexer->start)
      : new_token(TOKEN_LESS, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '>': return matches(lexer, '=')
      ? new_token(TOKEN_GREATER_EQUAL, &lexer->source[lexer->start], lexer->current - lexer->start)
      : new_token(TOKEN_GREATER, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '+': return new_token(TOKEN_PLUS, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '-': return new_token(TOKEN_MINUS, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '*': return new_token(TOKEN_STAR, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '/': return new_token(TOKEN_SLASH, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '%': return new_token(TOKEN_PERCENT, &lexer->source[lexer->start], lexer->current - lexer->start);
    case '\'': return handle_strings(lexer);
    default:
      if (c == '\0') return new_token(TOKEN_EOF, &lexer->source[lexer->start], lexer->current - lexer->start);
      return handle_identifiers(lexer);
  }
}
