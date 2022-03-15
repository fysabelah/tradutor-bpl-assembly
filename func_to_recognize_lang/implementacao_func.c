#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "def_func.h"

bool identify_functions(char *line, int size) {
    int func_dig, read_return, par_number1, par_number2, par_number3;
    char par_type1, par_type2, par_type3;

    read_return = sscanf(line, "function f%d p%c%d p%c%d p%c%d", &func_dig, &par_type1, &par_number1, &par_type2, &par_number2, &par_type3, &par_number3);

    if (read_return == 1) {
        printf("É uma função sem parâmetros\n");
    } 
    
    if (read_return == 3) {
        printf("É uma função de um parâmetro\n");
    }

    if (read_return == 5) {
        printf("É uma função de dois parâmetro\n");
    }

    if (read_return == 7) {
        printf("É uma função de três parâmetro\n");
    }
 
    //TODO: Tem que validar o tipo de dados inteiro ou array para validar as coisas
    
}

void validate_type(char type) {
    if (type == 'a') {
        //TODO: is array of integer
    } else {
        //TODO: is just integer
    }
}

bool identify_begin_variable_block(char *line, int size) {
    if (strncmp(line, "def", 3) == 0) {
        printf("Inicio do bloco da variavel\n");
    }
}

bool identify_end_variable_block(char *line, int size) {
    if (strncmp(line, "enddef", 6) == 0) {
        printf("Fim do bloco de variaveis\n");
    }
}

bool identify_local_var(char *line, int size) {
    int read_return_vector, read_return_integer, var_number, vet_number, vet_size;
    char par_type1, par_type2, par_type3;

    read_return_integer = sscanf(line, "var vi%d ", &var_number); 
    read_return_vector = sscanf(line, "vet va%d size ci%d", &vet_number, &vet_size);

    //TODO: Pode usar essa função aqui validate_type()
    if (read_return_integer == 1) {
        printf("A variavel é inteira\n");
    } else {
        if (read_return_vector == 2) {
            printf("A variavel é um vetor\n");
        }
    }
}

bool identify_function_body(char *line, int size) {
    int read_var_body;
    int num_var_op1, num_ass_type_op1, num_var_op2, num_ass_type_op2;
    char ass_type_op1, op, ass_type_op2;

    read_var_body = sscanf(line, "vi%d = %ci%d %c %ci%d", &num_var_op1, &ass_type_op1, &num_ass_type_op1, &op, &ass_type_op2, &num_ass_type_op2); 

    if (read_var_body == 3) {
        printf("É uma operação simples\n");
    } else if (read_var_body == 6) {
        printf("É uma operação expressão\n");
    }

    /*
    Com num_var é possível saber em quem atribuir; ass_type é o tipo que será passado
    vi1 = ci1 contante to var
    vi2 = vi1 var to var
    vi3 = pi2 param to param
    vi1 = pi1 + vi2 # vi1 = pi1 + vi2
    vi2 = vi1 * ci-5

    vai ter que se necessário fazer as validações também
    */
}

bool call_function(char *line, int size) {
    int read_call_func;
    int number_func_to_call, num1, num2, num3, assig_var_number;
    char type1, type2, type3, type_int_arr1, type_int_arr2, type_int_arr3, assig_var, assig_var_type;

    //Primeiro %c pode ser v,c ou p
    //Segundo %c pode ser a ou i
    //Terceiro pode ser número
    read_call_func = sscanf(line, "%c%c%d = call f%d %c%c%d %c%c%d %c%c%d", &assig_var, &assig_var_type, &assig_var_number, &number_func_to_call, &type1, &type_int_arr1, &num1, &type2, &type_int_arr2, &num2, &type3, &type_int_arr3, &num3);

    if (read_call_func >= 4) {
        printf("É uma chamada de função\n");
    }
}

bool get_array(char *line, int size) {
    int read_get_array, num_type_var, index, num_var_go;
    char type_var, type_var_go, int_or_array_var_go;

    read_get_array = sscanf(line, "get %ca%d index ci%d to %c%c%d", &type_var, &num_type_var, &index, &type_var_go, &int_or_array_var_go, &num_var_go);

    if (read_get_array == 6) {
        printf("Recupera o valor do vetor\n");
    }
}

bool set_array(char *line, int size) {
    int read_get_array, num_type_var, index, num_var_go;
    char type_var, type_var_go, int_or_array_var_go;

    read_get_array = sscanf(line, "set %ca%d index ci%d with %c%c%d", &type_var, &num_type_var, &index, &type_var_go, &int_or_array_var_go, &num_var_go);

    if (read_get_array == 6) {
        printf("Seta o valor no vetor\n");
    }
}

bool identify_conditional(char *line, int size) {
    int read_if, cond_number;
    char cond_type_op, cond_type;
    //Se recebe o if, então tem que avaliar os proximos valores
    read_if = sscanf(line, "if %c%c%d", &cond_type_op, &cond_type, &cond_number);

    if (read_if == 3) {
        printf("É um if\n");
        return true;
    }

    return false;
}

bool identify_end_conditional(char *line, int size) {
    return strncmp(line, "endif", 5) == 0;
}

bool identify_return_of_function(char *line, int size) {
    int read_return, num;
    char op_var_return, type_var;

    read_return = sscanf(line, "return %c%c%d", &op_var_return, &type_var, &num);

    return read_return == 3;
}