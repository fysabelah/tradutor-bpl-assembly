#include <stdio.h>
#include "func_to_recognize_lang/def_func.h"

#define LINESZ 256
#define SIZE_POINTER 8

void remove_newline(char*);

int main() {
    info_func dados_func;
    char line[LINESZ];
    int qtd_if = 0;
    var_locais variaveis_funcao;
    info_pilha info_pilha;

    printf(".data\n\n");
    printf(".text\n");

    while (fgets(line, LINESZ, stdin) != NULL) {
        printf("%s", line);
        remove_newline(line);

        /*
        Para resolver o problema de chamada de função, posso fazer com que
        todo parâmetro seja guardado na pilha. Assim, quando chamar, não vai se perder!
        Eu acho.
        */
        if (identify_functions(line, &dados_func)) {
            info_pilha.tamanho_pilha =  SIZE_POINTER * dados_func.quantidade_params;
            info_pilha.ja_construida = false;
            variaveis_funcao.quantidade = 0;  
        }

        identify_local_var(line, &variaveis_funcao);

        if (identify_end_variable_block(line) && !info_pilha.ja_construida) {
            info_pilha.ja_construida = true;

            printf("\t\tpushq %%rbp\n");
            printf("\t\tmovq %%rsp, %%rbp\n");

            info_pilha.tamanho_pilha = stack_size_check(&variaveis_funcao, info_pilha.tamanho_pilha);

            printf("\t\tsubq $%d, %%rsp\n\n", info_pilha.tamanho_pilha);

            save_func_params_on_stack(&dados_func);
            
            save_var_on_stack(info_pilha.tamanho_pilha, &variaveis_funcao);
        }

        /*Validar salvamento das variaveis e parametros na pilha*/
    }
}