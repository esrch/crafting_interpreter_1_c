#ifndef PARSER_H

# define PARSER_H

# include <stdbool.h>

# include "../token/token.h"
# include "../expression/expression.h"
# include "../errors/errors.h"

typedef struct s_parser
{
	t_token_list *token_list;
	int current;
}	t_parser;

// Parser.
t_expr *parse(t_token_list *token_list);

#endif