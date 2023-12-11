#ifndef ERRORS_H

# define ERRORS_H

# include <stdio.h>
# include <stdlib.h>

# include "../libft/libft.h"

void print_system_error(int exit_code);
void print_args_error(int exit_code);
void print_line_error(int line, char *message);

#endif