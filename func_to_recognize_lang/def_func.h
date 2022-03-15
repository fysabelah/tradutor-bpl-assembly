#include <stdbool.h>

bool identify_functions(char *line, int size);

bool identify_local_var(char *line, int size);

bool identify_begin_variable_block(char *line, int size);

bool identify_end_variable_block(char *line, int size);

bool identify_function_body(char *line, int size);

bool call_function(char *line, int size);

bool get_array(char *line, int size);

bool set_array(char *line, int size);

bool identify_conditional(char *line, int size);

bool identify_end_conditional(char *line, int size);

bool identify_return_of_function(char *line, int size);