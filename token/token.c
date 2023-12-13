#include "token.h"

t_token *token_construct(t_token_type type, char *lexeme, void *literal, int line)
{
	t_token *result;

	result = malloc(sizeof(t_token));
	if (!result)
		return (NULL);
	result->type = type;
	result->lexeme = lexeme;
	result->literal = literal;
	result->line = line;
	return (result);
}

void token_destruct(t_token **token_ptr)
{
	t_token *token;

	token = *token_ptr;
	if (!token)
		return ;
	free(token->lexeme);
	free(token->literal);
	free(token);
	*token_ptr = NULL;
}

static t_token *token_cpy_number(t_token *token, char *lexeme_cpy)
{
	double literal_value;
	double *literal_cpy;

	literal_cpy = malloc(sizeof(double));
	if (!literal_cpy)
	{
		free(lexeme_cpy);
		return (NULL);
	}

	literal_value = *(double*)token->literal;
	*literal_cpy = literal_value;
	
	return (token_construct(token->type, lexeme_cpy, literal_cpy, token->line));
}

static t_token *token_cpy_string(t_token *token, char *lexeme_cpy)
{
	char *s;
	char *literal_cpy;

	s = (char*)token->literal;
	literal_cpy = ft_strdup(s);
	if (!literal_cpy)
	{
		free(lexeme_cpy);
		return (NULL);
	}

	return (token_construct(token->type, lexeme_cpy, literal_cpy, token->line));
}

t_token *token_cpy(t_token *token)
{
	char *lexeme_cpy;

	lexeme_cpy = ft_strdup(token->lexeme);
	if (!lexeme_cpy)
		return (NULL);

	if (token->type == T_NUMBER && token->literal)
		return (token_cpy_number(token, lexeme_cpy));
	if (token->type == T_STRING && token->literal)
		return (token_cpy_string(token, lexeme_cpy));

	if (token->literal)
		ft_printf("token_cpy: Unreachable code.");

	return (token_construct(token->type, lexeme_cpy, NULL, token->line));
}

char *token_to_string(t_token *token)
{
	char *result;
	int size;
	char *token_type_str;

	token_type_str = token_type_to_string(token->type);
	if (!token_type_str)
		return (NULL);

	size = snprintf(NULL, 0, "%s %s",
		token_type_str, token->lexeme);

	result = malloc(sizeof(*result) * (size + 1));
	if (!result)
	{
		free(token_type_str);
		return (NULL);
	}

	snprintf(result, size + 1, "%s %s",
		token_type_str, token->lexeme);

	free(token_type_str);
	return (result);
}