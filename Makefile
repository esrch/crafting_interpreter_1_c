NAME = lox
SRCS = $(wildcard *.c */*.c)
OBJS = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Ilibft
LIBFLAGS = -Llibft -lft
SANITIZE_FLAGS = -g3 -fsanitize=address

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBFLAGS)

sanitize: $(OBJS)
	$(CC) $(CFLAGS) $(SANITIZE_FLAGS) $^ -o $(NAME) $(LIBFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re