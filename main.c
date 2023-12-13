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
	t_token_list *token_list;
	t_expr *ast;

	token_list = scan(script);
	if (!token_list)
		return (-1);

	ast = parse(token_list);
	token_list_destruct(&token_list);
	if (!ast)
		return (-1);
	
	print_ast(ast);

	expr_destruct(&ast);
	return (0);
}