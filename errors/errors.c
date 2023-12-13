#include "errors.h"

void print_system_error(int exit_code)
{
	perror(NULL);
	if (exit_code)
		exit(exit_code);
}

void print_args_error(int exit_code)
{
	ft_printf("Usage: clox [script]\n");
	if (exit_code)
		exit(exit_code);
}

void print_line_error(int line, char *message)
{
	if (!message)
		ft_printf("[line %d] Error\n", line);
	else
		ft_printf("[line %d] Error: %s\n", line, message);
	free(message);
}

void print_token_error(t_token *token, char *message)
{
	print_line_error(token->line, message);
}