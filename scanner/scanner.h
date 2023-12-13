#ifndef SCANNER_H

# define SCANNER_H

# include <stdlib.h>
# include <stdbool.h>

# include "../libft/libft.h"
# include "../token/token.h"
# include "../errors/errors.h"

typedef struct s_scanner
{
	char *source;
	int start;
	int current;
	int line;
}	t_scanner;

// Scanner.
t_token_list *scan(char *source);

#endif