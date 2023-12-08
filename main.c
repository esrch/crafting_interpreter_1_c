#include "lox.h"

static void runFile(char *path);
static void runPrompt(void);
static void run(char *script);

int main(int argc, char **argv)
{
	if (argc > 2) {
		print_and_exit(E_ARG_COUNT);
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
		print_and_exit(E_SYSTEM);
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

static void run(char *script)
{
	printf("Script:\n");
	printf("----------------------\n");
	printf("%s", script);
	printf("\n----------------------\n");
}