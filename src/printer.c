#include "stdio.h"
#include "ast.h"

void print_ast(tuna_ast_node* node, int indent) {
  switch (node->kind) {
    case AST_INTEGER_LITERAL:
      printf("%*s INTEGER: %li\n", indent, "", node->as.integer_literal);
      break;
    case AST_FLOAT_LITERAL:
      printf("%*s FLOAT: %f\n", indent, "", node->as.float_literal);
      break;
    case AST_UNARY_EXPRESSION:
      printf("%*s UNARY: %.*s\n", indent, "", node->as.unary.operator.length, node->as.unary.operator.lexeme);
      print_ast(node->as.unary.operand, indent + 2);
      break;
    case AST_BINARY_EXPRESSION:
      printf("%*s BINARY: %.*s\n", indent, "", node->as.binary.operator.length, node->as.binary.operator.lexeme);
      print_ast(node->as.binary.left, indent + 2);
      print_ast(node->as.binary.right, indent + 2);
      break;
  }
}
