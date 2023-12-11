#ifndef EXPRESSION_H

# define EXPRESSION_H

# include "../token/token.h"

typedef enum e_expr_type
{
	BINARY_EXPR,
	UNARY_EXPR,
	LITERAL_EXPR,
	GROUPING_EXPR
}	t_expr_type;

struct s_expr;

typedef struct s_binary_expr_members
{
	struct s_expr *left;
	struct s_expr *right;
	t_token *op;
}	t_binary_expr_members;

typedef struct s_unary_expr_members
{
	struct s_expr *expr;
	t_token *op;
}	t_unary_expr_members;

typedef struct s_literal_expr_members
{
	void *value;
}	t_literal_expr_members;

typedef struct s_grouping_expr_members
{
	struct s_expr *expr;
}	t_grouping_expr_members;

typedef union u_expr_members
{
	t_binary_expr_members binary;
	t_unary_expr_members unary;
	t_literal_expr_members literal;
	t_grouping_expr_members grouping;
}	t_expr_members;

typedef struct s_expr
{
	t_expr_type expr_type;
	t_expr_members members;
}	t_expr;

// Expression.
t_expr *binary_expr_construct(t_expr *left, t_token *op, t_expr *right);
t_expr *unary_expr_construct(t_token *op, t_expr *expr);
t_expr *literal_expr_construct(void *value);
t_expr *grouping_expr_construct(t_expr *expr);
void expr_destruct(t_expr **expr_ptr);


#endif