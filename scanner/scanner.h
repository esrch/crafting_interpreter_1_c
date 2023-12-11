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
t_scanner *scanner_construct(char *source);
void scanner_destruct(t_scanner **scanner_ptr);
t_token_list *scan_tokens(t_scanner *scanner);

#endif