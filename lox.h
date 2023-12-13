#ifndef LOX_H

# define LOX_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>

# include "errors/errors.h"
# include "token/token.h"
# include "expression/expression.h"
# include "scanner/scanner.h"
# include "parser/parser.h"

// File loader
char *load_script(char *path);

// Utils
char *append_str(char *initial_str, char *append_str, int append_size);
char *read_line_from_stdin(void);

#endif