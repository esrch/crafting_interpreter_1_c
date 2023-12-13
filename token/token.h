#ifndef TOKEN_H

# define TOKEN_H

# include <stdlib.h>
# include <string.h>
# include <stdio.h>

# include "../libft/libft.h"

typedef enum e_token_type
{
	// Single-character tokens.
	T_LEFT_PAREN,
	T_RIGHT_PAREN,
	T_LEFT_BRACE,
	T_RIGHT_BRACE,
	T_COMMA,
	T_DOT,
	T_MINUS,
	T_PLUS,
	T_SEMICOLON,
	T_SLASH,
	T_STAR,

	// One or two character tokens.
	T_BANG,
	T_BANG_EQUAL,
	T_EQUAL,
	T_EQUAL_EQUAL,
	T_GREATER,
	T_GREATER_EQUAL,
	T_LESS,
	T_LESS_EQUAL,

	// Literals.
	T_IDENTIFIER,
	T_STRING,
	T_NUMBER,

	// Keywords.
	T_AND,
	T_CLASS,
	T_ELSE,
	T_FALSE,
	T_FUN,
	T_FOR,
	T_IF,
	T_NIL,
	T_OR,
	T_PRINT,
	T_RETURN,
	T_SUPER,
	T_THIS,
	T_TRUE,
	T_VAR,
	T_WHILE,

	T_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *lexeme;
	void *literal;
	int line;
}	t_token;

typedef struct s_token_list
{
	t_token **list;
	int size;
	int count;
}	t_token_list;

// Token type.
char *token_type_to_string(t_token_type token_type);

// Token.
t_token *token_construct(t_token_type type, char *lexeme, void *literal, int line);
void token_destruct(t_token **token_ptr);
t_token *token_cpy(t_token *token);
char *token_to_string(t_token *token);

// Token list.
t_token_list *token_list_construct();
void token_list_destruct(t_token_list **token_list_ptr);
t_token *token_list_add(t_token_list *token_list, t_token *new_token);

#endif