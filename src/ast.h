#ifndef tuna_ast_h
#define tuna_ast_h

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "token.h"

typedef enum {
  AST_INTEGER_LITERAL,
  AST_FLOAT_LITERAL,
  AST_STRING_LITERAL,
  AST_BOOLEAN_LITERAL,
  AST_NULL_LITERAL,
  AST_ARRAY_LITERAL,
  AST_OBJECT_LITERAL,
  AST_IDENTIFIER,
  AST_UNARY_EXPRESSION,
  AST_BINARY_EXPRESSION,
  AST_BLOCK,
  AST_IF_EXPRESSION,
  AST_WHILE_EXPRESSION,
  AST_FOR_EXPRESSION,
  AST_FUNCTION_DECLARATION,
  AST_VARIABLE_DECLARATION,
  AST_FUNCTION_CALL,
  AST_OBJECT_ACCESS,
} tuna_ast_kind;

typedef struct tuna_ast_node {
  tuna_ast_kind kind;
  union {
    struct {
      size_t length;
      const char* data;
    } string;
    double float_literal;
    int64_t integer_literal;
    bool boolean_literal;

    struct {
      size_t length;
      struct tuna_ast_node* data;
    } array_literal;

    struct {
      size_t length;
      struct {
        const char* key;
        struct tuna_ast_node* value;
      }* data;
    } object_literal;

    struct {
      const char* id;
      size_t length;
    } identifier;

    struct {
      token operator;
      struct tuna_ast_node* operand;
    } unary;

    struct {
      struct tuna_ast_node* left;
      token operator;
      struct tuna_ast_node* right;
    } binary;

    struct {
      struct tuna_ast_node* expressions;
      size_t expression_count;
    } block;

    struct {
      struct tuna_ast_node* condition;
      struct tuna_ast_node* then_block;
      struct tuna_ast_node* else_block;
    } if_expression;

    struct {
      struct tuna_ast_node* condition;
      struct tuna_ast_node* block;
    } while_expression;

    struct {
      struct tuna_ast_node* initializer;
      struct tuna_ast_node* condition;
      struct tuna_ast_node* increment;
      struct tuna_ast_node* block;
    } for_expression;

    struct {
      const char* id;
      size_t length;

      struct tuna_ast_node* parameters;
      size_t parameter_count;

      struct tuna_ast_node* block;
    } function_declaration;

    struct {
      const char* id;
      size_t length;

      struct tuna_ast_node* initializer;
    } variable_declaration;

    struct {
      const char* id;
      size_t length;

      struct tuna_ast_node* parameters;
      size_t parameter_count;
    } function_call;

    struct {
      struct tuna_ast_node* object;
      
      const char* id;
      size_t length;
    } object_access;
  } as;
} tuna_ast_node;

#endif
