#include "parser.h"

static t_expr *expression(t_parser *parser);
static t_expr *equality(t_parser *parser);
static t_expr *comparison(t_parser *parser);
static t_token *advance(t_parser *parser);
static t_token *peek(t_parser *parser);
static t_token *previous(t_parser *parser);
static bool match(t_parser *parser, t_token_type token_type);
static bool check(t_parser *parser, t_token_type token_type);
static bool is_at_end(t_parser *parser);

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
	free(parser);
	*parser_ptr = NULL;
}

static t_expr *expression(t_parser *parser)
{
	return (equality(parser));
}

static t_expr *equality(t_parser *parser)
{
	t_expr *expr;

	expr = comparison(parser);

	while (match(parser, T_BANG_EQUAL) || match(parser, T_EQUAL_EQUAL))
	{
        t_token *operator = previous(parser);
        t_expr *right = comparison(parser);
        expr = binary_expr_construct(expr, operator, right);
	}

    return (expr);
}

static t_expr *comparison(t_parser *parser)
{

}

static t_token *advance(t_parser *parser)
{
    if (!is_at_end(parser))
        parser->current++;
    return previous(parser);
}

static t_token *peek(t_parser *parser)
{
    return (parser->token_list->list[parser->current]);
}

static t_token *previous(t_parser *parser)
{
    return (parser->token_list->list[parser->current - 1]);
}

static bool match(t_parser *parser, t_token_type token_type)
{
    if (check(parser, token_type)) {
        advance(parser);
        return (true);
    }

    return (false);
}

static bool check(t_parser *parser, t_token_type token_type)
{
    if (!is_at_end(parser))
        parser->current++;
    return (peek(parser)->type == token_type);
}

static bool is_at_end(t_parser *parser)
{
    return (peek(parser)->type == T_EOF);
}