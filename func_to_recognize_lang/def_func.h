#include <stdbool.h>
typedef struct info_params {
    bool isInt;
    char nome[15];
    int pos_stack;
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
int stack_size_check(var_locais*, int);
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
void save_func_params_on_stack(info_func *info);