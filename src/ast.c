#include <stdlib.h>

#include "ast.h"

void free_ast(tuna_ast_node* root) {
  switch (root->kind) {
    case AST_INTEGER_LITERAL:
    case AST_FLOAT_LITERAL:
    case AST_BOOLEAN_LITERAL:
      printf("DEBUG: (%s:%i %s) FREE - %li\n", __FILE__, __LINE__, __func__, sizeof(root));
      free(root);
      break;
    case AST_UNARY_EXPRESSION:
      printf("DEBUG: (%s:%i %s) FREE - %li\n", __FILE__, __LINE__, __func__, sizeof(root->as.unary.operand));
      free(root->as.unary.operand);
      printf("DEBUG: (%s:%i %s) FREE - %li\n", __FILE__, __LINE__, __func__, sizeof(root));
      free(root);
    case AST_BINARY_EXPRESSION:
      printf("DEBUG: (%s:%i %s) FREE - %li\n", __FILE__, __LINE__, __func__, sizeof(root->as.binary.left));
      free(root->as.binary.left);
      printf("DEBUG: (%s:%i %s) FREE - %li\n", __FILE__, __LINE__, __func__, sizeof(root->as.binary.right));
      free(root->as.binary.right);
      printf("DEBUG: (%s:%i %s) FREE - %li\n", __FILE__, __LINE__, __func__, sizeof(root));
      free(root);
      break;
  }
}
