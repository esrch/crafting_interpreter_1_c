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
