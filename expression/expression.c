#include "expression.h"

t_expr *binary_expr_construct(t_expr *left, t_token *op, t_expr *right)
{
	t_expr *binary_expr;

	binary_expr = malloc(sizeof(*binary_expr));
	if (!binary_expr)
		return (NULL);
	
	binary_expr->expr_type = BINARY_EXPR;
	binary_expr->members.binary.left = left;
	binary_expr->members.binary.op = op;
	binary_expr->members.binary.right = right;

	return (binary_expr);
}

void binary_expr_destruct(t_expr **expr_ptr)
{
	t_expr *expr;

	expr = *expr_ptr;
	expr_destruct(&expr->members.binary.left);
	token_destruct(&expr->members.binary.op);
	expr_destruct(&expr->members.binary.right);

	free(expr);
	*expr_ptr = NULL;
}

void unary_expr_destruct(t_expr **expr_ptr)
{
	t_expr *expr;

	expr = *expr_ptr;
	token_destruct(&expr->members.unary.op);
	expr_destruct(&expr->members.unary.expr);

	free(expr);
	*expr_ptr = NULL;
}

void literal_expr_destruct(t_expr **expr_ptr)
{
	t_expr *expr;

	expr = *expr_ptr;
	free(expr->members.literal.value);

	free(expr);
	*expr_ptr = NULL;
}

void grouping_expr_destruct(t_expr **expr_ptr)
{
	t_expr *expr;

	expr = *expr_ptr;
	expr_destruct(&expr->members.grouping.expr);

	free(expr);
	*expr_ptr = NULL;
}

t_expr *unary_expr_construct(t_token *op, t_expr *expr)
{
	t_expr *unary_expr;

	unary_expr = malloc(sizeof(*unary_expr));
	if (!unary_expr)
		return (NULL);
	
	unary_expr->expr_type = UNARY_EXPR;
	unary_expr->members.unary.op = op;
	unary_expr->members.unary.expr = expr;

	return (unary_expr);
}

t_expr *literal_expr_construct(void *value)
{
	t_expr *literal_expr;

	literal_expr = malloc(sizeof(*literal_expr));
	if (!literal_expr)
		return (NULL);

	literal_expr->expr_type = LITERAL_EXPR;
	literal_expr->members.literal.value = value;
	
	return (literal_expr);
}

t_expr *grouping_expr_construct(t_expr *expr)
{
	t_expr *grouping_expr;

	grouping_expr = malloc(sizeof(*grouping_expr));
	if (!grouping_expr)
		return (NULL);

	grouping_expr->expr_type = GROUPING_EXPR;
	grouping_expr->members.grouping.expr = expr;

	return (grouping_expr);
}

void expr_destruct(t_expr **expr_ptr)
{
	if ((*expr_ptr)->expr_type == BINARY_EXPR)
		binary_expr_destruct(expr_ptr);
	else if ((*expr_ptr)->expr_type == UNARY_EXPR)
		unary_expr_destruct(expr_ptr);
	else if ((*expr_ptr)->expr_type == LITERAL_EXPR)
		literal_expr_destruct(expr_ptr);
	else if ((*expr_ptr)->expr_type == GROUPING_EXPR)
		grouping_expr_destruct(expr_ptr);
	else
	{
		ft_printf("Should never happen. Invalid expression type in expr_destruct().\n");
		exit(1);
	}
}