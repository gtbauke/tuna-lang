#include <stdio.h>

#include "lexer.h"
#include "token.h"

int main(void) {
  tuna_lexer* lexer = new_lexer("if 4 > 5 { print(5 $ 5) } else { print('hello') }");

  while (!is_lexer_at_end(lexer)) {
    token token = next_token(lexer);
    printf("%i - %.*s\n", token.kind, (int)token.length, token.lexeme);
  }

  free_lexer(lexer);
  return 0;
}
