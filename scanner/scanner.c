#include "scanner.h"

static int scan_token(t_scanner *scanner, t_token_list *token_list);
static t_token *add_token(t_scanner *scanner, t_token_list *token_list,
	t_token_type token_type, void *literal);
static char advance(t_scanner *scanner);
static char peek(t_scanner *scanner);
static bool match(t_scanner *scanner, char expected);
static bool is_at_end(t_scanner *scanner);

t_scanner *scanner_construct(char *source)
{
	t_scanner *result;

	result = malloc(sizeof(*result));
	if (!result)
		return (NULL);
	result->source = source;
	result->start = 0;
	result->current = 0;
	result->line = 1;
	return (result);
}

void scanner_destruct(t_scanner **scanner_ptr)
{
	t_scanner *scanner;

	scanner = *scanner_ptr;
	if (!scanner)
		return ;
	free(scanner->source);
	free(scanner);
	*scanner_ptr = NULL;
}

t_token_list *scan_tokens(t_scanner *scanner)
{
	t_token_list *token_list;
	bool had_error;

	token_list = token_list_construct();
	if (!token_list)
		return (NULL);
	
	had_error = false;
	while (!is_at_end(scanner))
	{
		scanner->start = scanner->current;
		if (scan_token(scanner, token_list) < 0)
			had_error = true;
	}

	if (had_error)
	{
		token_list_destruct(&token_list);
		return (NULL);
	}

	// Add EOF token.
	if (!token_list_add(token_list, token_construct(T_EOF, NULL, NULL, scanner->line)))
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
	else
	{
		print_line_error(scanner->line, ft_strdup("Unexpected character."));
		return (-1);
	}

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