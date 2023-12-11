#include "lox.h"

static void runFile(char *path);
static void runPrompt(void);
static int run(char *script);

int main(int argc, char **argv)
{
	if (argc > 2) {
		print_args_error(1);
	} else if (argc == 2) {
		runFile(argv[1]);
	} else {
		runPrompt();
	}
}

static void runFile(char *path)
{
	char *script;

	script = load_script(path);
	if (!script)
		print_system_error(1);
	run(script);
	free(script);
}

static void runPrompt(void)
{
	char *line;

	write(1, "> ", 2);
	line = read_line_from_stdin();
	while (line)
	{
		run(line);
		free(line);
		write(1, "> ", 2);
		line = read_line_from_stdin();
	}
	printf("\n\nThank you and goodbye!\n");
}

static int run(char *script)
{
	int i;
	t_scanner *scanner;
	t_token_list *token_list;
	char *token_string;

	scanner = scanner_construct(script);
	if (!scanner)
	{
	 	print_system_error(0);
		return (-1);
	}
	token_list = scan_tokens(scanner);
	if (!token_list)
	{
		scanner_destruct(&scanner);
		return (-1);
	}
	
	i = 0;
	while (i < token_list->count)
	{
		token_string = token_to_string(token_list->list[i]);
		if (token_string)
			ft_printf("%s\n", token_string);
		free(token_string);
		i++;
	}

	scanner_destruct(&scanner);
	token_list_destruct(&token_list);
	return (0);
}