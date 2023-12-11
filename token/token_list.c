#include "token.h"

t_token_list *token_list_construct()
{
	t_token_list *result;

	result = malloc(sizeof(*result));
	if (!result)
		return (NULL);
	
	result->count = 0;
	result->size = 8;
	result->list = malloc(sizeof(t_token*) * 8);
	if (!result->list)
	{
		free(result);
		return (NULL);
	}
	return (result);
}

void token_list_destruct(t_token_list **token_list_ptr)
{
	t_token_list *token_list;

	token_list = *token_list_ptr;
	if (!token_list)
		return ;
	for (int i = 0; i < token_list->count; i++)
		token_destruct(&token_list->list[i]);
	free(token_list->list);
	free(token_list);
	*token_list_ptr = NULL;
}

static t_token_list *token_list_expand(t_token_list *token_list)
{
	t_token **old_list;
	t_token **new_list;
	int new_size;

	old_list = token_list->list;
	new_size = token_list->size * 2;
	new_list = realloc(old_list, new_size * sizeof(t_token*));
	if (!new_list)
		return (NULL);
	token_list->list = new_list;
	token_list->size = new_size;
	return (token_list);
}

t_token *token_list_add(t_token_list *token_list, t_token *new_token)
{
	if (!new_token)
		return (NULL);
	
	if (token_list->count >= token_list->size
		&& !token_list_expand(token_list))
		return (NULL);
	
	token_list->list[token_list->count++] = new_token;
	return (new_token);
}