#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "ast.h"
#include "printer.h"

int main(void) {
  tuna_lexer* lexer = new_lexer("45.67 + 5 <= 67.78 and not 56");
  tuna_parser* parser = new_parser(lexer);

  tuna_ast_node* program = parse_program(parser);
  print_ast(program, 0);

  free_ast(program);
  free_parser(parser);
  free_lexer(lexer);
  return 0;
}
