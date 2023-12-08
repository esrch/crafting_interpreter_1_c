#include "lox.h"

void print_error(t_error error)
{
	if (error == E_SYSTEM)
		perror(NULL);
	else if (error == E_ARG_COUNT)
		printf("Usage: clox [script]\n");
}

void print_and_exit(t_error error)
{
	print_error(error);
	exit(1);
}
