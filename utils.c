#include "lox.h"

char *append_str(char *initial_str, char *append_str, int append_size)
{
	char *result;
	int total_size;

	if (!initial_str)
	{
		total_size = append_size;
		result = malloc(sizeof(*result) * (total_size + 1));
		strncpy(result, append_str, append_size);
		result[total_size] = 0;
		return (result);
	}

	total_size = strlen(initial_str) + append_size;
	result = realloc(initial_str, total_size + 1);
	if (!result)
	{
		free(initial_str);
		return (NULL);
	}
	strncat(result, append_str, append_size);
	result[total_size] = 0;
	free(initial_str);
	return (result);
}

char *read_line_from_stdin(void)
{
	char buffer[1024];
	int bytes;
	char *line;
	int eol;

	eol = 0;
	line = NULL;
	bytes = read(0, buffer, 1024);
	while (bytes > 0)
	{
		if (buffer[bytes - 1] == '\n')
			eol = 1;
		line = append_str(line, buffer, bytes - eol);
		if (eol)
			return (line);
		bytes = read(0, buffer, 1024);
	}
	if (bytes < 0)
	{
		free(line);
		return (NULL);
	}
	free(line);
	return (NULL);
}