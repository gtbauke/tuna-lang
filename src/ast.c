#include <stdlib.h>

#include "ast.h"

void free_ast(tuna_ast_node* root) {
  switch (root->kind) {
    case AST_INTEGER_LITERAL:
    case AST_FLOAT_LITERAL:
    case AST_BOOLEAN_LITERAL:
      free(root);
      break;
    case AST_UNARY_EXPRESSION:
      free(root->as.unary.operand);
      free(root);
    case AST_BINARY_EXPRESSION:
      free(root->as.binary.left);
      free(root->as.binary.right);
      free(root);
      break;
  }
}
