#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "biblioteca/biblioteca.h"

#define LINESZ 256

int main() {
    info_func dados_func;
    char line[LINESZ];
    int qtd_if = 0;
    var_locais variaveis_funcao;
    info_pilha info_pilha;

    printf(".data\n\n");
    printf(".text\n");

    while (fgets(line, LINESZ, stdin) != NULL) {
        remove_nova_linha(line);
        
        if (identifica_funcoes(line, &dados_func)) {
            info_pilha.tamanho_pilha = 0;
            info_pilha.ja_construida = false;
            variaveis_funcao.quantidade = 0;  
        }

        identify_local_var(line, &variaveis_funcao);
        
        if (identify_end_variable_block(line) && !info_pilha.ja_construida) {
            info_pilha.ja_construida = true;

            printf("\t\tpushq %%rbp\n");
            printf("\t\tmovq %%rsp, %%rbp\n");

            info_pilha.tamanho_pilha = stack_size_check(&variaveis_funcao);

            printf("\t\tsubq $%d, %%rsp\n\n", info_pilha.tamanho_pilha);
            
            save_var_on_stack(info_pilha.tamanho_pilha, &variaveis_funcao);
        }

        identify_assignment(line, &dados_func, &variaveis_funcao);
        get_array(line, &dados_func, &variaveis_funcao);
        set_array(line, &dados_func, &variaveis_funcao);
        identify_return_of_function(line, &dados_func, &variaveis_funcao);
        identify_conditional(line, &dados_func, &variaveis_funcao, qtd_if);

        if (identify_end_conditional(line)) {
            printf("\t\tend_if%d:\n", qtd_if);
            qtd_if++;
        }
    }

    return 0;
}