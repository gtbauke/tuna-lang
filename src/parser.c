#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "parser.h"

tuna_parser* new_parser(tuna_lexer* lexer) {
  tuna_parser* parser = malloc(sizeof(tuna_parser));
  parser->lexer = lexer;
  parser->had_error = false;
  return parser;
}

void free_parser(tuna_parser* parser) {
  free(parser);
}

static void advance(tuna_parser* parser) {
  parser->previous = parser->current;
  parser->current = next_token(parser->lexer);
}

static bool is_parser_at_end(tuna_parser* parser) {
  return parser->current.kind == TOKEN_EOF;
}

static bool matches(tuna_parser* parser, token_kind kind) {
  if (is_parser_at_end(parser) || parser->current.kind != kind) {
    return false;
  }

  advance(parser);
  return true;
}

static void error(tuna_parser* parser, const char* message) {
  parser->had_error = true;
  fprintf(stderr, "Error: %s\n", message);
  exit(1);
}

static void expect(tuna_parser* parser, token_kind kind, const char* message) {
  if (!matches(parser, kind)) error(parser, message);
}

// Some forward declarations
static tuna_ast_node* parse_expression(tuna_parser* parser, precedence precedence);
static parse_rule* get_rule(token_kind kind);

static tuna_ast_node* parse_integer(tuna_parser* parser) {
  int64_t value = strtol(parser->current.lexeme, NULL, 10);
  tuna_ast_node* node = malloc(sizeof(tuna_ast_node));

  node->kind = AST_INTEGER_LITERAL;
  node->as.integer_literal = value;

  return node;
}

static tuna_ast_node* parse_float(tuna_parser* parser) {
  double value = strtod(parser->current.lexeme, NULL);
  tuna_ast_node* node = malloc(sizeof(tuna_ast_node));

  node->kind = AST_FLOAT_LITERAL;
  node->as.float_literal = value;

  return node;
}

static tuna_ast_node* parse_binary(tuna_parser* parser, tuna_ast_node* left) {
  tuna_ast_node* node = malloc(sizeof(tuna_ast_node));

  node->kind = AST_BINARY_EXPRESSION;
  node->as.binary.left = left;
  node->as.binary.operator = parser->current;
  node->as.binary.right = parse_expression(parser, get_rule(parser->current.kind)->precedence + 1);

  return node;
}

static tuna_ast_node* parse_unary(tuna_parser* parser) {
  tuna_ast_node* node = malloc(sizeof(tuna_ast_node));

  node->kind = AST_UNARY_EXPRESSION;
  node->as.unary.operator = parser->current;
  node->as.unary.operand = parse_expression(parser, PREC_UNARY);

  return node;
}

static tuna_ast_node* parse_boolean(tuna_parser* parser) {
  tuna_ast_node* node = malloc(sizeof(tuna_ast_node));

  node->kind = AST_BOOLEAN_LITERAL;
  node->as.boolean_literal = parser->current.kind == TOKEN_TRUE;

  return node;
}

static tuna_ast_node* parse_grouping(tuna_parser* parser) {
  tuna_ast_node* node = parse_expression(parser, PREC_NONE);
  expect(parser, TOKEN_CLOSE_PAREN, "Expected ')' after expression.");

  return node;
}

static parse_rule parse_rules[] = {
  [TOKEN_INTEGER]       = {parse_integer, NULL, PREC_NONE},
  [TOKEN_FLOAT]         = {parse_float, NULL, PREC_NONE},
  [TOKEN_TRUE]          = {parse_boolean, NULL, PREC_NONE},
  [TOKEN_FALSE]         = {parse_boolean, NULL, PREC_NONE},
  [TOKEN_OPEN_PAREN]    = {parse_grouping, NULL, PREC_NONE},
  [TOKEN_PLUS]          = {NULL, parse_binary, PREC_TERM},
  [TOKEN_MINUS]         = {parse_unary, parse_binary, PREC_TERM},
  [TOKEN_STAR]          = {NULL, parse_binary, PREC_FACTOR},
  [TOKEN_SLASH]         = {NULL, parse_binary, PREC_FACTOR},
  [TOKEN_PERCENT]       = {NULL, parse_binary, PREC_FACTOR},
  [TOKEN_EQUAL_EQUAL]   = {NULL, parse_binary, PREC_EQUALITY},
  [TOKEN_BANG_EQUAL]    = {NULL, parse_binary, PREC_EQUALITY},
  [TOKEN_GREATER]       = {NULL, parse_binary, PREC_COMPARISON},
  [TOKEN_GREATER_EQUAL] = {NULL, parse_binary, PREC_COMPARISON},
  [TOKEN_LESS]          = {NULL, parse_binary, PREC_COMPARISON},
  [TOKEN_LESS_EQUAL]    = {NULL, parse_binary, PREC_COMPARISON},
  [TOKEN_AND]           = {NULL, parse_binary, PREC_AND},
  [TOKEN_OR]            = {NULL, parse_binary, PREC_OR},
  [TOKEN_NOT]           = {parse_unary, NULL, PREC_UNARY},
  [TOKEN_BANG]          = {parse_unary, NULL, PREC_UNARY},
  [TOKEN_EOF]           = {NULL, NULL, PREC_NONE},
};

static parse_rule* get_rule(token_kind kind) {
  return &parse_rules[kind];
}

static tuna_ast_node* parse_expression(tuna_parser* parser, precedence precedence) {
  // printf("previous token: %i\n", parser->previous.kind);
  // printf("current token: %i\n", parser->current.kind);

  advance(parser);

  // printf("previous token: %i\n", parser->previous.kind);
  // printf("current token: %i\n", parser->current.kind);

  parse_prefix_fn prefix_rule = get_rule(parser->current.kind)->prefix;
  if (prefix_rule == NULL) {
    error(parser, "No prefix parselet.");
  }

  tuna_ast_node* left = prefix_rule(parser);
  while (precedence <= get_rule(parser->current.kind)->precedence) {
    // printf("previous token: %i\n", parser->previous.kind);
    // printf("current token: %i\n", parser->current.kind);

    advance(parser);

    // printf("previous token: %i\n", parser->previous.kind);
    // printf("current token: %i\n", parser->current.kind);

    if (parser->current.kind == TOKEN_EOF) break;

    parse_infix_fn infix_rule = get_rule(parser->current.kind)->infix;
    if (infix_rule == NULL) {
      error(parser, "No infix parselet.");
    }

    left = infix_rule(parser, left);
  }

  return left;
}

tuna_ast_node* parse_program(tuna_parser* parser) {
  return parse_expression(parser, PREC_NONE);
}
