#include "token.h"

token new_token(token_kind kind, const char* lexeme, size_t length, int line, int column, const char* file_path) {
  return (token) {
    .kind = kind,
    .lexeme = lexeme,
    .length = length,
    .line = line,
    .column = column,
    .file_path = file_path,
  };
}

const char* token_kind_as_string(token_kind kind) {
  switch (kind) {
    case TOKEN_OPEN_PAREN: return "(";
    case TOKEN_CLOSE_PAREN: return ")";
    case TOKEN_OPEN_BRACKET: return "[";
    case TOKEN_CLOSE_BRACKET: return "]";
    case TOKEN_OPEN_CURLY: return "{";
    case TOKEN_CLOSE_CURLY: return "}";
    case TOKEN_COMMA: return ",";
    case TOKEN_COLON: return ":";
    case TOKEN_DOT: return ".";
    case TOKEN_PLUS: return "+";
    case TOKEN_MINUS: return "-";
    case TOKEN_STAR: return "*";
    case TOKEN_SLASH: return "/";
    case TOKEN_PERCENT: return "%";
    case TOKEN_BANG: return "!";
    case TOKEN_EQUAL: return "=";
    case TOKEN_EQUAL_EQUAL: return "==";
    case TOKEN_BANG_EQUAL: return "!=";
    case TOKEN_LESS: return "<";
    case TOKEN_LESS_EQUAL: return "<=";
    case TOKEN_GREATER: return ">";
    case TOKEN_GREATER_EQUAL: return ">=";

    case TOKEN_LET: return "let";
    case TOKEN_FUN: return "fun";
    case TOKEN_IF: return "if";
    case TOKEN_ELSE: return "else";
    case TOKEN_TRUE: return "true";
    case TOKEN_FALSE: return "false";
    case TOKEN_RETURN: return "return";
    case TOKEN_WHILE: return "while";
    case TOKEN_FOR: return "for";
    case TOKEN_BREAK: return "break";
    case TOKEN_CONTINUE: return "continue";
    case TOKEN_NULL: return "null";
    case TOKEN_AND: return "and";
    case TOKEN_OR: return "or";
    case TOKEN_NOT: return "not";

    case TOKEN_IDENTIFIER: return "identifier";
    case TOKEN_STRING: return "string";
    case TOKEN_INTEGER: return "integer";
    case TOKEN_FLOAT: return "float";
    case TOKEN_ERROR: return "error";
    case TOKEN_EOF: return "eof";
    default: return "unknown";
  }
}
