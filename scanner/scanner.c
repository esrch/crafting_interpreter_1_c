#include "scanner.h"

static int scan_token(t_scanner *scanner, t_token_list *token_list);
static t_token *add_token(t_scanner *scanner, t_token_list *token_list,
	t_token_type token_type, void *literal);
static int string(t_scanner *scanner, t_token_list *token_list);
static int number(t_scanner *scanner, t_token_list *token_list);
static int identifier(t_scanner *scanner, t_token_list *token_list);
static char advance(t_scanner *scanner);
static char peek(t_scanner *scanner);
static char peek_next(t_scanner *scanner);
static bool match(t_scanner *scanner, char expected);
static bool is_at_end(t_scanner *scanner);

t_token_list *scan(char *source)
{
	t_scanner scanner;
	t_token_list *token_list;
	bool had_error;

	token_list = token_list_construct();
	if (!token_list)
		return (NULL);
	
	scanner.source = source;
	scanner.start = 0;
	scanner.current = 0;
	scanner.line = 1;

	had_error = false;
	while (!is_at_end(&scanner))
	{
		scanner.start = scanner.current;
		if (scan_token(&scanner, token_list) < 0)
			had_error = true;
	}

	if (had_error)
	{
		token_list_destruct(&token_list);
		return (NULL);
	}

	// Add EOF token.
	if (!token_list_add(token_list, token_construct(T_EOF, NULL, NULL, scanner.line)))
	{
		token_list_destruct(&token_list);
		return (NULL);
	}
	
	return (token_list);
}

static int scan_token(t_scanner *scanner, t_token_list *token_list)
{
	char c;

	c = advance(scanner);
	if (c == '(')
		add_token(scanner, token_list, T_LEFT_PAREN, NULL);
	else if (c == ')')
		add_token(scanner, token_list, T_RIGHT_PAREN, NULL);
	else if (c == '{')
		add_token(scanner, token_list, T_LEFT_BRACE, NULL);
	else if (c == '}')
		add_token(scanner, token_list, T_RIGHT_BRACE, NULL);
	else if (c == ',')
		add_token(scanner, token_list, T_COMMA, NULL);
	else if (c == '.')
		add_token(scanner, token_list, T_DOT, NULL);
	else if (c == '-')
		add_token(scanner, token_list, T_MINUS, NULL);
	else if (c == '+')
		add_token(scanner, token_list, T_PLUS, NULL);
	else if (c == ';')
		add_token(scanner, token_list, T_SEMICOLON, NULL);
	else if (c == '*')
		add_token(scanner, token_list, T_STAR, NULL);
	else if (c == '!' && match(scanner, '='))
		add_token(scanner, token_list, T_BANG_EQUAL, NULL);
	else if (c == '!')
		add_token(scanner, token_list, T_BANG, NULL);
	else if (c == '=' && match(scanner, '='))
		add_token(scanner, token_list, T_EQUAL_EQUAL, NULL);
	else if (c == '=')
		add_token(scanner, token_list, T_EQUAL, NULL);
	else if (c == '<' && match(scanner, '='))
		add_token(scanner, token_list, T_LESS_EQUAL, NULL);
	else if (c == '<')
		add_token(scanner, token_list, T_LESS, NULL);
	else if (c == '>' && match(scanner, '='))
		add_token(scanner, token_list, T_GREATER_EQUAL, NULL);
	else if (c == '>')
		add_token(scanner, token_list, T_GREATER, NULL);
	else if (c == '/' && match(scanner, '/'))
	{
		while (peek(scanner) != '\n' && !is_at_end(scanner))
			advance(scanner);
	}
	else if (c == '/')
		add_token(scanner, token_list, T_SLASH, NULL);
	else if (c == ' ' || c == '\r' || c == '\t')
		;
	else if (c == '\n')
		scanner->line++;
	else if (c == '"')
		return (string(scanner, token_list));
	else if (ft_isdigit(c))
		return (number(scanner, token_list));
	else if (ft_isalpha(c) || c == '_')
		return (identifier(scanner, token_list));
	else
	{
		print_line_error(scanner->line, ft_strdup("Unexpected character."));
		return (-1);
	}

	return (0);
}

static int string(t_scanner *scanner, t_token_list *token_list)
{
	char *value;

	while (peek(scanner) != '"' && !is_at_end(scanner)) {
		if (peek(scanner) == '\n')
			scanner->line++;
		advance(scanner);
	}

	if (is_at_end(scanner))
	{
		print_line_error(scanner->line, ft_strdup("Unterminated string."));
		return (-1);
	}

	// Consume the closing ".
	advance(scanner);

	// Trim the surrounding quotes.
	value = ft_substr(scanner->source, scanner->start + 1, scanner->current - scanner->start - 2);
	if (!value)
	{
		print_system_error(0);
		return (-1);
	}

	if (!add_token(scanner, token_list, T_STRING, value))
	{
		free(value);
		return (-1);
	}
	return (0);
}

static int number(t_scanner *scanner, t_token_list *token_list)
{
	double *value_ptr;

	while (ft_isdigit(peek(scanner)))
		advance(scanner);
	
	if (peek(scanner) == '.' && ft_isdigit(peek_next(scanner)))
	{
		advance(scanner);
		while (ft_isdigit(peek(scanner)))
			advance(scanner);
	}

	value_ptr = malloc(sizeof(double));
	if (!value_ptr)
		return (-1);
	*value_ptr = strtod(scanner->source + scanner->start, NULL);
	if (!add_token(scanner, token_list, T_NUMBER, value_ptr))
	{
		free(value_ptr);
		return (-1);
	}
	return (0);
}

static int keyword(t_scanner *scanner, t_token_list *token_list, bool *is_keyword)
{
	int i;
	int keyword_len;
	static char *keywords[] = {"and", "class", "else", "false", "for", "fun", "if", "nil",
		"or", "print", "return", "super", "this", "true", "var", "while", NULL};
	static t_token_type token_types[] = {T_AND, T_CLASS, T_ELSE, T_FALSE, T_FOR, T_FUN, T_IF, T_NIL,
		T_OR, T_PRINT, T_RETURN, T_SUPER, T_THIS, T_TRUE, T_VAR, T_WHILE};
	
	i = 0;
	keyword_len = scanner->current - scanner->start;
	while (keywords[i])
	{
		if (ft_strncmp(keywords[i], scanner->source + scanner->start, keyword_len) == 0)
		{
			*is_keyword = true;
			if (!add_token(scanner, token_list, token_types[i], NULL))
				return (-1);
			return (0);
		}
		i++;
	}

	*is_keyword = false;
	return (0);
}

static int identifier(t_scanner *scanner, t_token_list *token_list)
{
	bool is_keyword;
	int result;

	while (ft_isalnum(peek(scanner)) || peek(scanner) == '_')
		advance(scanner);
	
	result = keyword(scanner, token_list, &is_keyword);
	if (is_keyword)
		return (result);

	if (!add_token(scanner, token_list, T_IDENTIFIER, NULL))
		return (-1);
	return (0);
}

static t_token *add_token(t_scanner *scanner, t_token_list *token_list,
	t_token_type token_type, void *literal)
{
	char *lexeme;
	t_token *new_token;

	lexeme = ft_substr(scanner->source, scanner->start, scanner->current - scanner->start);
	if (!lexeme)
		return (NULL);

	new_token = token_construct(token_type, lexeme, literal, scanner->line);
	if (!token_list_add(token_list, new_token))
	{
		free(lexeme);
		token_destruct(&new_token);
		return (NULL);
	}

	return (new_token);
}

static char advance(t_scanner *scanner)
{
	return (scanner->source[scanner->current++]);
}

static char peek(t_scanner *scanner)
{
	return (scanner->source[scanner->current]);
}

static char peek_next(t_scanner *scanner)
{
	if (scanner->source[scanner->current] == 0
		|| scanner->source[scanner->current + 1] == 0)
		return (0);
	
	return (scanner->source[scanner->current + 1]);
}

static bool match(t_scanner *scanner, char expected)
{
	if (is_at_end(scanner))
		return (false);
	if (scanner->source[scanner->current] != expected)
		return (false);
	
	scanner->current++;
	return (true);
}

static bool is_at_end(t_scanner *scanner)
{
	return (scanner->source[scanner->current] == 0);
}