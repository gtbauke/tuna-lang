#ifndef tuna_parser_h
#define tuna_parser_h

#include "ast.h"
#include "lexer.h"
#include "token.h"

typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT,  // =
  PREC_OR,          // or
  PREC_AND,         // and
  PREC_EQUALITY,    // == !=
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * /
  PREC_UNARY,       // ! -
  PREC_CALL,        // . ()
  PREC_PRIMARY
} precedence;

typedef struct {
  tuna_lexer* lexer;
  token current;
  token previous;

  // TODO: errors
  bool had_error;
} tuna_parser;

typedef tuna_ast_node* (*parse_prefix_fn)(tuna_parser* parser);
typedef tuna_ast_node* (*parse_infix_fn)(tuna_parser* parser, tuna_ast_node* left);

typedef struct {
  parse_prefix_fn prefix;
  parse_infix_fn infix;
  precedence precedence;
} parse_rule;

tuna_parser* new_parser(tuna_lexer* lexer);
void free_parser(tuna_parser* parser);

tuna_ast_node* parse_program(tuna_parser* parser);

#endif
