#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "func_to_recognize_lang/def_func.h"

#define LINESZ 256

void remove_newline(char*);

int main() {
    char line[LINESZ];

    while (fgets(line, LINESZ, stdin) != NULL) {
        printf("%s", line);
        remove_newline(line);
        identify_functions(line, LINESZ);
        identify_begin_variable_block(line, LINESZ);
        identify_local_var(line, LINESZ);
        identify_end_variable_block(line, LINESZ);
        identify_function_body(line, LINESZ);
        call_function(line, LINESZ);
        get_array(line, LINESZ);
        set_array(line, LINESZ);

        if (identify_conditional(line, LINESZ)) {
          while (!identify_end_conditional(line, LINESZ)){
            get_array(line, LINESZ);
            set_array(line, LINESZ);
            identify_function_body(line, LINESZ);
            fgets(line, LINESZ, stdin) != NULL;
            printf("%s\n", line);
          }
        }

        if(identify_return_of_function(line, LINESZ)) {
          printf("identificado return\n");
        }

        if(strncmp(line, "end", 5) == 0) {
          printf("Fim da função\n");
        }

        printf("\n");
    }
}

// Remove o '\n' do fim da linha
void remove_newline(char *ptr) {
  while (*ptr) {
    if (*ptr == '\n')
      *ptr = 0;
    else
      ptr++;
  }
}