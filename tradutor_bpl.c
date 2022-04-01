#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define LINESZ 256

typedef struct formato_funcao {
    int quantidade_params;
    bool tipo_param1_int;
    bool tipo_param2_int;
    bool tipo_param3_int;
} info_func;

typedef struct info_variaveis {
    char tipo[10];
    char nome[15];
    int valor;
    int tamanho;
} var;

typedef struct var_locais {
    var variaveis[5];
    int quantidade;
} var_locais;

typedef struct info_pilha {
    int tamanho_pilha;
    bool ja_construida;
} info_pilha;


void remove_newline(char*);
bool identify_functions(char*, info_func*);
bool validate_type(char);
bool identify_begin_variable_block(char*);
bool identify_local_var(char*, var_locais*);
bool identify_end_variable_block(char*);
int verifica_tamanho_pilha(var_locais*);
void print_var_pilha(int, var_locais*);

//Esse código vai funcionar para uma função. Tem que adaptar
int main() {
    info_func dados_func;
    char line[LINESZ];
    int qtd_local_var;
    var_locais variaveis_funcao;
    info_pilha info_pilha;

    printf(".text\n");

    while (fgets(line, LINESZ, stdin) != NULL) {
        remove_newline(line);
        
        if (identify_functions(line, &dados_func)) {
            info_pilha.tamanho_pilha = 0;
            info_pilha.ja_construida = false;
            variaveis_funcao.quantidade = 0;    
        }

        identify_local_var(line, &variaveis_funcao);
        
        if (identify_end_variable_block(line) && !info_pilha.ja_construida) {
            info_pilha.ja_construida = true;

            printf("\t\tpushq %%rbp\n");
            printf("\t\tmovq %%rsp, %%rbp\n");

            info_pilha.tamanho_pilha = verifica_tamanho_pilha(&variaveis_funcao);

            printf("\t\tsubq $%d, %%rsp\n\n", info_pilha.tamanho_pilha);
            
            print_var_pilha(info_pilha.tamanho_pilha, &variaveis_funcao);
        }

        //Vou começar a olhar agora a chamada de função

    }

    return 0;
}

void print_var_pilha(int tam_pilha, var_locais *var) {
    int tam_atual = 0, tam_var;

    for (int i = 0; i < (*var).quantidade; i++) {
        tam_var = strcmp((*var).variaveis[i].tipo, "var") == 0 ? 4 : 
            4 * (*var).variaveis[i].tamanho;

        if (tam_atual == 0) {
            printf("\t\t#%s: (%d)%%rbp\n", (*var).variaveis[i].nome, 0);
        } else {
            while (tam_atual % 4 != 0) {
                tam_atual++;
            }
            
            printf("\t\t#%s: (%d)%%rbp\n", (*var).variaveis[i].nome, -1 * tam_atual);
        }
        
        tam_atual += tam_var;
    }
}

int verifica_tamanho_pilha(var_locais *var) {
    int tamanho_variaveis = 0;

    for (int i = 0; i < (*var).quantidade; i++) {
        tamanho_variaveis += strcmp((*var).variaveis[i].tipo, "var") == 0 ? 4 : 
            4 * (*var).variaveis[i].tamanho;
    }

    //Acaba que não retorna o tamanho das variaveis e sim o tamanho da pilha
    if (tamanho_variaveis <= 16) {
        return 16;
    } 

    while (tamanho_variaveis % 16 != 0) {
        tamanho_variaveis += 16 - tamanho_variaveis % 16;
    }

    return tamanho_variaveis;
}

void remove_newline(char *ptr) {
  while (*ptr) {
    if (*ptr == '\n')
      *ptr = 0;
    else
      ptr++;
  }
}

bool identify_local_var(char *line, var_locais *info_var) {
    int read_return_vector, read_return_integer, var_number, vet_number, vet_size;
    char par_type1, par_type2, par_type3;

    read_return_integer = sscanf(line, "var vi%d ", &var_number); 
    read_return_vector = sscanf(line, "vet va%d size ci%d", &vet_number, &vet_size);

    char codigo_var[10];

    if (read_return_integer == 1) {
        (*info_var).quantidade ++;

        int pos = (*info_var).quantidade - 1;
        sprintf(codigo_var, "%d", var_number); 
        strcpy((*info_var).variaveis[pos].tipo, "var");
        strcpy((*info_var).variaveis[pos].nome, "vi");
        strcat((*info_var).variaveis[pos].nome, codigo_var);
        
        return true;
    } 
    
    if (read_return_vector == 2) {
        (*info_var).quantidade ++;

        int pos = (*info_var).quantidade - 1;
        sprintf(codigo_var, "%d", vet_number); 
        strcpy((*info_var).variaveis[pos].tipo, "vet");
        strcpy((*info_var).variaveis[pos].nome, "va");
        strcat((*info_var).variaveis[pos].nome, codigo_var);
        (*info_var).variaveis[pos].tamanho = vet_size;

        return true;
    }

    return false;
}

bool identify_functions(char *line, info_func *info) {
    int func_dig, read_return, par_number1, par_number2, par_number3;
    char par_type1, par_type2, par_type3;

    read_return = sscanf(line, "function f%d p%c%d p%c%d p%c%d", &func_dig, &par_type1, &par_number1, &par_type2, &par_number2, &par_type3, &par_number3);

    char nome_func[50] = "f";
    char codigo[50];
    sprintf(codigo, "%d", func_dig); 
    strcat(nome_func, codigo);

    //print declaração da função
    if (read_return >= 1) {
        printf(".globl %s:\n", nome_func);
        printf("\t%s:\n", nome_func);
        return true;
    }

    if (read_return == 1) {
        (*info).quantidade_params = 0;
        return true;
    } 
    
    if (read_return == 3) {
        (*info).quantidade_params = 1;
        (*info).tipo_param1_int = validate_type(par_type1);
        return true;
    }

    if (read_return == 5) {
        (*info).quantidade_params = 2;
        (*info).tipo_param1_int = validate_type(par_type1);
        (*info).tipo_param2_int = validate_type(par_type2);
        return true;
    }

    if (read_return == 7) {
        (*info).quantidade_params = 3;
        (*info).tipo_param1_int = validate_type(par_type1);
        (*info).tipo_param2_int = validate_type(par_type2);
        (*info).tipo_param3_int = validate_type(par_type3);
        return true;
    }

    return false;
}

bool validate_type(char type) {
    if (type == 'a') {
        return false;
    } 
    
    return true;
}

bool identify_begin_variable_block(char *line) {
    if (strncmp(line, "def", 3) == 0) {
        return true;
    }

    return false;
}

bool identify_end_variable_block(char *line) {
    if (strncmp(line, "enddef", 6) == 0) {
        return true;
    }

    return false;
}