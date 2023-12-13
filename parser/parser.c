#include "parser.h"

static t_expr *expression(t_parser *parser);
static t_expr *equality(t_parser *parser);
static t_expr *comparison(t_parser *parser);
static t_expr *term(t_parser *parser);
static t_expr *factor(t_parser *parser);
static t_expr *unary(t_parser *parser);
static t_expr *primary(t_parser *parser);
static t_token *advance(t_parser *parser);
static t_token *peek(t_parser *parser);
static t_token *previous(t_parser *parser);
static bool match(t_parser *parser, t_token_type token_type);
static t_token *consume(t_parser *parser, t_token_type token_type, char *message);
static bool check(t_parser *parser, t_token_type token_type);
static bool is_at_end(t_parser *parser);
// static void synchronize(t_parser *parser);
static bool *malloc_bool(bool value);

t_expr *parse(t_token_list *token_list)
{
	t_parser parser;

	parser.token_list = token_list;
	parser.current = 0;
	return (expression(&parser));
}

static t_expr *expression(t_parser *parser)
{
	return (equality(parser));
}

static t_expr *equality(t_parser *parser)
{
	t_expr *expr;

	expr = comparison(parser);
	if (!expr)
		return (NULL);

	while (match(parser, T_BANG_EQUAL) || match(parser, T_EQUAL_EQUAL))
	{
        t_token *operator = token_cpy(previous(parser));
		if (!operator)
		{
			expr_destruct(&expr);
			return (NULL);
		}
        t_expr *right = comparison(parser);
		if (!right)
		{
			token_destruct(&operator);
			expr_destruct(&expr);
			return (NULL);
		}
        expr = binary_expr_construct(expr, operator, right);
	}

    return (expr);
}

static t_expr *comparison(t_parser *parser)
{
	t_expr *expr;

	expr = term(parser);
	if (!expr)
		return (NULL);

	while (match(parser, T_GREATER) || match(parser, T_GREATER_EQUAL)
		|| match(parser, T_LESS) || match(parser, T_LESS_EQUAL))
	{
		t_token *operator = token_cpy(previous(parser));
		if (!operator)
		{
			expr_destruct(&expr);
			return (NULL);
		}
		t_expr *right = term(parser);
		if (!right)
		{
			token_destruct(&operator);
			expr_destruct(&expr);
			return (NULL);
		}
		expr = binary_expr_construct(expr, operator, right);
	}

	return (expr);
}

static t_expr *term(t_parser *parser)
{
	t_expr *expr;

	expr = factor(parser);
	if (!expr)
		return (NULL);

	while (match(parser, T_MINUS) || match(parser, T_PLUS))
	{
		t_token *operator = token_cpy(previous(parser));
		if (!operator)
		{
			expr_destruct(&expr);
			return (NULL);
		}
		t_expr *right = factor(parser);
		if (!right)
		{
			token_destruct(&operator);
			expr_destruct(&expr);
			return (NULL);
		}
		expr = binary_expr_construct(expr, operator, right);
	}

	return (expr);
}

static t_expr *factor(t_parser *parser)
{
	t_expr *expr;

	expr = unary(parser);
	if (!expr)
		return (NULL);

	while (match(parser, T_SLASH) || match(parser, T_STAR))
	{
		t_token *operator = token_cpy(previous(parser));
		if (!operator)
		{
			expr_destruct(&expr);
			return (NULL);
		}
		t_expr *right = unary(parser);
		if (!right)
		{
			token_destruct(&operator);
			expr_destruct(&expr);
			return (NULL);
		}
		expr = binary_expr_construct(expr, operator, right);
	}

	return (expr);
}

static t_expr *unary(t_parser *parser)
{
	t_expr *right;

	if (match(parser, T_BANG) || match(parser, T_MINUS))
	{
		t_token *operator = token_cpy(previous(parser));
		if (!operator)
			return (NULL);
		right = unary(parser);	
		if (!right)
		{
			token_destruct(&operator);
			return (NULL);
		}
		return (unary_expr_construct(operator, right));
	}

	return (primary(parser));
}

static t_expr *primary(t_parser *parser)
{
	t_expr *expr;
	double *double_literal;

	if (match(parser, T_FALSE))
		return (literal_expr_construct(LITERAL_BOOL, malloc_bool(false)));
	if (match(parser, T_TRUE))
		return (literal_expr_construct(LITERAL_BOOL, malloc_bool(true)));
	if (match(parser, T_NIL))
		return (literal_expr_construct(LITERAL_NIL, NULL));
	if (match(parser, T_NUMBER))
	{
		double_literal = malloc(sizeof(double));
		*double_literal = *(double*)(previous(parser)->literal);
		return (literal_expr_construct(LITERAL_NUMBER, double_literal));
	}
	if (match(parser, T_STRING))
		return (literal_expr_construct(LITERAL_STRING, ft_strdup(previous(parser)->literal)));

	if (match(parser, T_LEFT_PAREN))
	{
		expr = expression(parser);
		if (!consume(parser, T_RIGHT_PAREN, ft_strdup("Expected ')' after expression.")))
		{
			expr_destruct(&expr);
			return (NULL);
		}
		return (grouping_expr_construct(expr));	
	}

	print_token_error(peek(parser), ft_strdup("Expected expression."));
	return (NULL);
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

static t_token *consume(t_parser *parser, t_token_type token_type, char *message)
{
	if (check(parser, token_type))
		return (advance(parser));
	
	print_token_error(peek(parser), message);
	return (NULL);
}

static bool check(t_parser *parser, t_token_type token_type)
{
    if (is_at_end(parser))
        return (false);
    return (peek(parser)->type == token_type);
}

static bool is_at_end(t_parser *parser)
{
    return (peek(parser)->type == T_EOF);
}

/*
static void synchronize(t_parser *parser)
{
	advance(parser);

	while (!is_at_end(parser))
	{
		if (previous(parser)->type == T_SEMICOLON)
			return ;
	
		switch(peek(parser)->type)
		{
			case T_CLASS:
			case T_FUN:
			case T_VAR:
			case T_FOR:
			case T_IF:
			case T_WHILE:
			case T_PRINT:
			case T_RETURN:
				return;
		}

		advance(parser);
	}
}
*/

static bool *malloc_bool(bool value)
{
	bool *result;

	result = malloc(sizeof(bool));
	if (!result)
		return (NULL);
	
	*result = value;
	return (result);
}