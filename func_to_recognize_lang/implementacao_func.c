#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "def_func.h"

#define SIZE_POINTER 8

void remove_newline(char *ptr) {
  while (*ptr) {
    if (*ptr == '\n')
      *ptr = 0;
    else
      ptr++;
  }
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
        printf(".globl %s\n", nome_func);
        printf("\t%s:\n", nome_func);
    }
    
    if (read_return == 1) {
        info->quantidade_params = 0;

        return true;
    } 
    
    if (read_return == 3) {
        info->quantidade_params = 1;
        
        (*info).info_params[0].isInt = validate_type(par_type1);
        create_name_param(info, (*info).info_params[0].isInt, 0, par_number1);
        
        return true;
    }

    if (read_return == 5) {
        info->quantidade_params = 2;
        
        (*info).info_params[0].isInt = validate_type(par_type1);
        create_name_param(info, (*info).info_params[0].isInt, 0, par_number1);

        (*info).info_params[1].isInt = validate_type(par_type2);
        create_name_param(info, (*info).info_params[1].isInt, 1, par_number2);

        return true;
    }

    if (read_return == 7) {
        info->quantidade_params = 3;

        (*info).info_params[0].isInt = validate_type(par_type1);
        create_name_param(info, (*info).info_params[0].isInt, 0, par_number1);

        (*info).info_params[1].isInt = validate_type(par_type2);
        create_name_param(info, (*info).info_params[1].isInt, 1, par_number2);

        (*info).info_params[2].isInt = validate_type(par_type3);
        create_name_param(info, (*info).info_params[1].isInt, 3, par_number3);

        return true;
    }

    return false;
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

void create_name_param(info_func *info, bool isInt, int indice, int num){
    char codigo[10];

    sprintf(codigo, "%d", num);

    if ((*info).info_params[indice].isInt) {
        strcpy((*info).info_params[indice].nome, "pi");
        strcat((*info).info_params[indice].nome, codigo);
    } else {
        strcpy((*info).info_params[indice].nome, "pa");
        strcat((*info).info_params[indice].nome, codigo);
    }
}

void save_func_params_on_stack(info_func *info) {
    if ((*info).quantidade_params != 0) {
        int size = SIZE_POINTER, i;

        for (i = 0; i < (*info).quantidade_params; i++) {
            char type = (*info).info_params[i].isInt ? 'i' : 'a';
            char reg[10];

            return_appropriate_register(i, type, reg);
            (*info).info_params[i].pos_stack = size;
            
            printf("\t\tmovl %%%s, %d(%%rbp)\n", reg, -1 * size);
            
            size += SIZE_POINTER;
        }      
    }
}

void save_var_on_stack(int tam_pilha, var_locais *var) {
    int tam_atual = 0, tam_var;

    for (int i = 0; i < (*var).quantidade; i++) {
        tam_var = strcmp((*var).variaveis[i].tipo, "var") == 0 ? 4 : 
            4 * (*var).variaveis[i].tamanho;

        tam_atual += tam_var;

        while (tam_atual % 4 != 0) {
            tam_atual++;
        }

        printf("\t\t#%s: %d(%%rbp)\n", (*var).variaveis[i].nome, -1 * tam_atual);
            (*var).variaveis[i].posicao_pilha = tam_atual;
    }

    printf("\n");
}

void return_appropriate_register(int posicao_param, char tipo_param, char *end) {
    char reg[5];
    
    if (posicao_param == 0) {
        strcpy(reg, "rdi");
    } else {
        if (posicao_param == 1) {
            strcpy(reg, "rsi");
        } else {
            if (posicao_param == 2) {
                strcpy(reg, "rdx");
            }
        }
    }

    if (validate_type(tipo_param)) {
        reg[0] = 'e';
    }

    strcpy(end, reg);
}

bool validate_type(char type) {
    if (type == 'a') {
        return false;
    } 
    
    return true;
}

int stack_size_check(var_locais *var, int size_params) {
    int tamanho_variaveis = size_params;

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