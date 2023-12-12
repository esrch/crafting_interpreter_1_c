#include "parser.h"

// static t_expr *expression(t_parser *parser);
// static t_expr *equality(t_parser *parser);

t_parser *parser_construct(t_token_list *token_list)
{
	t_parser *parser;

	parser = malloc(sizeof(*parser));
	if (!parser)
		return (NULL);
	
	parser->token_list = token_list;
	parser->current = 0;

	return (parser);
}

void parser_destruct(t_parser **parser_ptr)
{
	t_parser *parser;

	parser = *parser_ptr;
	token_list_destruct(&parser->token_list);
	free(parser);
	*parser_ptr = NULL;
}
/*
static t_expr *expression(t_parser *parser)
{
	return (equality(parser));
}

static t_expr *equality(t_parser *parser)
{

}
*/