#ifndef PARSER_H

# define PARSER_H

# include <stdbool.h>

# include "../token/token.h"
# include "../expression/expression.h"

typedef struct s_parser
{
	t_token_list *token_list;
	int current;
}	t_parser;

// Parser.
t_parser *parser_construct(t_token_list *token_list);
void parser_destruct(t_parser **parser_ptr);

#endif