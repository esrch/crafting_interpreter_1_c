#ifndef LOX_H

# define LOX_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>

typedef enum e_error
{
	E_SYSTEM,
	E_ARG_COUNT
}	t_error;

// File loader
char *load_script(char *path);

// Errors
void print_error(t_error error);
void print_and_exit(t_error error);

// Utils
char *append_str(char *initial_str, char *append_str, int append_size);
char *read_line_from_stdin(void);

#endif