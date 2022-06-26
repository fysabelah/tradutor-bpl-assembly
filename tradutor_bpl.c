#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define LINESZ 256

typedef struct info_params {
    bool isInt;
    char nome[15];
} info_params;

typedef struct formato_funcao {
    int quantidade_params;
    info_params info_params[3];
} info_func;
typedef struct info_variaveis {
    char tipo[10];
    char nome[15];
    int valor;
    int tamanho;
    int posicao_pilha;
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
int stack_size_check(var_locais*);
void save_var_on_stack(int, var_locais*);
bool identify_assignment(char*, info_func*, var_locais*);
int search_variable_stack_position(char [], var_locais *);
void create_name_param(info_func*, bool, int, int);
void mount_name_var(char *, char, char, int);
int return_parameter_position(info_func*, char *);
void return_appropriate_register(int, char, char*);
bool identify_return_of_function(char*, info_func*, var_locais*);
bool get_array(char*, info_func*, var_locais*);
bool set_array(char*, info_func*, var_locais*);
bool identify_end_conditional(char *);
bool identify_conditional(char*, info_func*, var_locais*, int);

int main() {
    info_func dados_func;
    char line[LINESZ];
    int qtd_if = 0;
    var_locais variaveis_funcao;
    info_pilha info_pilha;

    printf(".data\n\n");
    printf(".text\n");

    while (fgets(line, LINESZ, stdin) != NULL) {
        remove_newline(line);
        
        if (identify_functions(line, &dados_func)) {
            info_pilha.tamanho_pilha = 0;
            info_pilha.ja_construida = false;
            variaveis_funcao.quantidade = 0;  
            qtd_if = 0;
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

bool identify_conditional(char *line, info_func *info_func, var_locais *var, int qtd_if) {
    int read_if, cond_number;
    char cond_type_op, cond_type;
    
    read_if = sscanf(line, "if %c%c%d", &cond_type_op, &cond_type, &cond_number);

    if (read_if == 3) {
        char var_to_comper[10];
        mount_name_var(var_to_comper, cond_type_op, cond_type, cond_number);

        int pos = search_variable_stack_position(var_to_comper, var);

        if (pos != -1) {
            printf("\t\tmovl %d(%%rbp), %%r15d\n", pos * -1);
        } else {
            pos = return_parameter_position(info_func, var_to_comper);

            if (pos != -1) {
                char reg[10];
                return_appropriate_register(pos, cond_type, reg);
                printf("\t\tmovl %%%s, %%r15d\n", reg);
            } else {
                printf("\t\tmovl $%d, %%r15d\n", cond_number);
            }
        }

        printf("\t\tcmpl $0, %%r15d\n");
        printf("\t\tje end_if%d\n", qtd_if);

        return true;
    }

    return false;
}

bool identify_end_conditional(char *line) {
    return strncmp(line, "endif", 5) == 0;
}

bool get_array(char *line, info_func *info_func, var_locais *var) {
    int read_get_array, num_type_var, index, num_var_go;
    char type_var, type_var_go, int_or_array_var_go;

    read_get_array = sscanf(line, "get %ca%d index ci%d to %c%c%d", &type_var, &num_type_var, &index, &type_var_go, &int_or_array_var_go, &num_var_go);

    if (read_get_array == 6) {
        char nome_array[10], nome_var_atribute[10];
        mount_name_var(nome_array, type_var, 'a', num_type_var);
        mount_name_var(nome_var_atribute, type_var_go, int_or_array_var_go, num_var_go);

        int pos = search_variable_stack_position(nome_array, var);

        printf("\t\t#%s = %s[%d]\n", nome_var_atribute, nome_array, index);

        //Pegando array
        if (pos != -1) {
            printf("\t\tleaq %d(%%rbp), %%r12\n", pos * -1);
        } else {
            pos = return_parameter_position(info_func, nome_array);
            char reg[10];
            return_appropriate_register(pos, 'a', reg);
            printf("\t\tmovq %%%s, %%r12\n", reg);
        }

        printf("\t\tmovabs $%d, %%r9\n", index);
        printf("\t\timulq $4, %%r9\n");
        printf("\t\taddq %%r12, %%r9\n");
        printf("\t\tmovl (%%r9), %%r8d\n");

        //Pegando variavel atribuir
        pos = search_variable_stack_position(nome_var_atribute, var);

        if (pos != -1) {
            printf("\t\tmovl %%r8d, %d(%%rbp)\n", -1 * pos);
        } else {
            pos = return_parameter_position(info_func, nome_var_atribute);
            char reg[10];
            return_appropriate_register(pos, int_or_array_var_go, reg);
            printf("\t\tmovl %%r8d, %%%s\n", reg);
        }

        printf("\n");

        return true;
    }

    return false;
}

bool set_array(char *line, info_func *info_func, var_locais *var) {
    int read_set_array, num_type_var, index, num_var_go;
    char type_var, type_var_go, int_or_array_var_go;

    read_set_array = sscanf(line, "set %ca%d index ci%d with %c%c%d", &type_var, &num_type_var, &index, &type_var_go, &int_or_array_var_go, &num_var_go);

    if (read_set_array == 6) {
        char nome_array[10], nome_var_set[10];
        mount_name_var(nome_array, type_var, 'a', num_type_var);
        mount_name_var(nome_var_set, type_var_go, int_or_array_var_go, num_var_go);

        int pos = search_variable_stack_position(nome_array, var);

        if (pos != -1) {
            printf("\t\tleaq %d(%%rbp), %%r9\n", pos * -1);
        } else {
            pos = return_parameter_position(info_func, nome_array);
            char reg[10];
            return_appropriate_register(pos, 'a', reg);
            printf("\t\tmovq %%%s, %%r9\n", reg);
        }
        
        printf("\t\tmovabs $%d, %%r11\n", index);
        printf("\t\timulq $4, %%r11\n");
        printf("\t\taddq %%r9, %%r11\n");

        pos = search_variable_stack_position(nome_var_set, var);

        if (pos != -1) {
            printf("\t\tmovl %d(%%rbp), %%r15d\n", -1 * pos);
            printf("\t\tmovl %%r15d, (%%r11)\n");
        } else {
            pos = return_parameter_position(info_func, nome_var_set);
            
            if (pos != -1) {
                char reg[10];
                return_appropriate_register(pos, type_var, reg);
                printf("\t\tmovl %%%s, (%%r11)\n", reg);
            } else {
                printf("\t\tmovl $%d, (%%r11)\n", num_var_go);
            }
        }

        printf("\n");

        return true;
    }

    return false;
}

bool identify_return_of_function(char *line, info_func *info_func, var_locais *var) {
    int read_return, num;
    char op_var_return, type_var;

    read_return = sscanf(line, "return %c%c%d", &op_var_return, &type_var, &num);
    
    if (read_return == 3) {
        printf("\n");

        char var_to_return[10];

        mount_name_var(var_to_return, op_var_return, type_var, num);

        int pos = search_variable_stack_position(var_to_return, var);

        if (pos != -1) {
            printf("\t\tmovl %d(%%rbp), %%eax\n", -1 * pos);
        } else {
            pos = return_parameter_position(info_func, var_to_return);

            if (pos != -1) {
                char reg[10];
                return_appropriate_register(pos, type_var, reg);
                printf("\t\tmovl %%%s, %%eax\n", reg);
            } else {
                printf("\t\tmovl $%d, %%eax\n", num);
            }
        }

        printf("\t\tleave\n");
        printf("\t\tret\n");

        return true;
    }

    return false;
}

void mount_name_var(char *pont_recebe, char char1, char char2, int num_param) {
    char codigo[10];
    char nome[4] = {char1, char2};

    sprintf(codigo, "%d", num_param); 
    strcat(nome, codigo);
    strcpy(pont_recebe, nome);
}

int return_parameter_position(info_func* info, char *nome_param) {
    for (int i = 0; i < (*info).quantidade_params; i++) {
        if (strcmp(nome_param, (*info).info_params[i].nome) == 0) {
            return i;
        }
    }

    return -1;
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

bool identify_assignment(char *line, info_func *info_func, var_locais *var) {
    int read_var_body, pos_var;
    int num_var_op1, num_ass_type_op1, num_var_op2, num_ass_type_op2;
    char ass_type_op1, op, ass_type_op2;

    read_var_body = sscanf(line, "vi%d = %ci%d %c %ci%d", &num_var_op1, &ass_type_op1, &num_ass_type_op1, &op, &ass_type_op2, &num_ass_type_op2); 

    if (read_var_body == 3) {
        char var_recebe[10], var_ser_atribuido[10];

        mount_name_var(var_recebe, 'v', 'i', num_var_op1);
        mount_name_var(var_ser_atribuido, ass_type_op1, 'i', num_ass_type_op1);

        int pos_var_recebe = search_variable_stack_position(var_recebe, var);
        int pos_var_ser_atribuida = search_variable_stack_position(var_ser_atribuido, var);

        switch (ass_type_op1) {
            case 'c':
                printf("\t\tmovl $%d, %d(%%rbp)\n", num_ass_type_op1, -1 * pos_var_recebe);
                break;
            case 'v':
                printf("\t\tleaq %d(%%rbp), %%r9\n", -1 * pos_var_ser_atribuida);
                printf("\t\tmovl (%%r9), %%r8d\n");
                printf("\t\tmovl %%r8d, %d(%%rbp)\n", -1 * pos_var_recebe);
                break;
            case 'p':
                pos_var_ser_atribuida = return_parameter_position(info_func, var_ser_atribuido);
                
                char reg[10];
                return_appropriate_register(pos_var_ser_atribuida, 'i', reg);
                printf("\t\tmovl %%%s, %d(%%rbp)\n", reg, -1 * pos_var_recebe);

                break;
        }

        printf("\n");
    } else if (read_var_body == 6) {
        char var_recebe[10], var_op1[10], var_op2[10];
        int pos_var_ser_atribuida;

        mount_name_var(var_recebe, 'v', 'i', num_var_op1);
        mount_name_var(var_op1, ass_type_op1, 'i', num_ass_type_op1);
        mount_name_var(var_op2, ass_type_op2, 'i', num_ass_type_op2);

        int pos_var_recebe = search_variable_stack_position(var_recebe, var);

        //Primeiro operando
        switch (ass_type_op1) {
            case 'c':
                printf("\t\tmovl $%d, %%r9d\n", num_ass_type_op1);
                break;
            case 'v':
                pos_var_ser_atribuida = search_variable_stack_position(var_op1, var);

                printf("\t\tleaq %d(%%rbp), %%r8\n", -1 * pos_var_ser_atribuida);
                printf("\t\tmovl (%%r8), %%r9d\n");
                break;
            case 'p':
                pos_var_ser_atribuida = return_parameter_position(info_func, var_op1);
                
                char reg[10];
                return_appropriate_register(pos_var_ser_atribuida, ass_type_op1, reg);
                printf("\t\tmovl %%%s, %%r9d\n", reg);

                break;
        }

        //Segundo operando
        switch (ass_type_op2) {
            case 'c':
                printf("\t\tmovl $%d, %%r10d\n", num_ass_type_op2);
                break;
            case 'v':
                pos_var_ser_atribuida = search_variable_stack_position(var_op2, var);

                printf("\t\tleaq %d(%%rbp), %%r8\n", -1 * pos_var_ser_atribuida);
                printf("\t\tmovl (%%r8), %%r10d\n");
                break;
            case 'p':
                pos_var_ser_atribuida = return_parameter_position(info_func, var_op2);
                
                char reg[10];
                return_appropriate_register(pos_var_ser_atribuida, ass_type_op2, reg);
                printf("\t\tmovl %%%s, %%r10d\n", reg);

                break;
        }

        //valor necessários estao em r9d e r10d
        switch (op) {
            case '+':
                printf("\t\taddl %%r9d, %%r10d\n");
                printf("\t\tmovl %%r10d, %d(%%rbp)\n", -1 * pos_var_recebe);
                break;
            case '*':
                printf("\t\timull %%r9d, %%r10d\n");
                printf("\t\tmovl %%r10d, %d(%%rbp)\n", -1 * pos_var_recebe);
                break;
            case '-':
                printf("\t\tsubl %%r10d, %%r9d\n");
                printf("\t\tmovl %%r9d, %d(%%rbp)\n", -1 * pos_var_recebe);
                break;
            case '/':
                printf("\t\tmovl %%r9d, %%eax\n");
                printf("\t\tcltd\n");
                printf("\t\tidivl %%r10d\n");
                printf("\t\tmovl %%eax, %d(%%rbp)\n", -1 * pos_var_recebe);
                break;
        }

        printf("\n");
    }

    return false;
}

int search_variable_stack_position(char nome_var[], var_locais *dados_var) {
    for (int i = 0; i < (*dados_var).quantidade; i ++) {
        if (strcmp(nome_var, (*dados_var).variaveis[i].nome) == 0) {
            return (*dados_var).variaveis[i].posicao_pilha;
        }
    }

    return -1;
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

int stack_size_check(var_locais *var) {
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