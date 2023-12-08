#include "lox.h"

static char *load_script_from_fd(int fd);

char *load_script(char *path)
{
	int fd;
	char *script;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	script = load_script_from_fd(fd);
	close(fd);
	return (script);
}

static char *load_script_from_fd(int fd)
{
	char buffer[1024];
	int bytes;
	char *file_contents;

	file_contents = NULL;
	bytes = read(fd, buffer, 1024);
	while (bytes > 0)
	{
		file_contents = append_str(file_contents, buffer, bytes);
		if (!file_contents)
			return (NULL);
		bytes = read(fd, buffer, 1024);
	}
	if (bytes < 0)
	{
		free(file_contents);
		return (NULL);
	}
	return (file_contents);
}
