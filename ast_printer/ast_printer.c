#include "ast_printer.h"

static void print_expr(t_expr *expr);
static void print_binary_expr(t_expr *expr);
static void print_unary_expr(t_expr *expr);
static void print_literal_expr(t_expr *expr);
static void print_grouping_expr(t_expr *expr);
static void print_literal_bool(bool *value);
static void print_literal_number(double *value);
static void print_literal_string(char *value);

void print_ast(t_expr *ast)
{
	print_expr(ast);
	ft_printf("\n");
}

static void print_expr(t_expr *expr)
{
	if (expr->expr_type == BINARY_EXPR)
		print_binary_expr(expr);
	else if (expr->expr_type == UNARY_EXPR)
		print_unary_expr(expr);
	else if (expr->expr_type == LITERAL_EXPR)
		print_literal_expr(expr);
	else if (expr->expr_type == GROUPING_EXPR)
		print_grouping_expr(expr);
	else
		ft_printf("print_expr: Unreachable code.");
}

static void print_binary_expr(t_expr *expr)
{
	ft_printf("(%s ", expr->members.binary.op->lexeme);
	print_expr(expr->members.binary.left);
	ft_printf(" ");
	print_expr(expr->members.binary.right);
	ft_printf(")");
}

static void print_unary_expr(t_expr *expr)
{
	ft_printf("(%s ", expr->members.unary.op->lexeme);
	print_expr(expr->members.unary.expr);
	ft_printf(")");
}

static void print_literal_expr(t_expr *expr)
{
	if (expr->members.literal.type == LITERAL_BOOL)
		print_literal_bool(expr->members.literal.value);
	else if (expr->members.literal.type == LITERAL_NIL)
		ft_printf("NIL");
	else if (expr->members.literal.type == LITERAL_NUMBER)
		print_literal_number(expr->members.literal.value);
	else if (expr->members.literal.type == LITERAL_STRING)
		print_literal_string(expr->members.literal.value);
}

static void print_grouping_expr(t_expr *expr)
{
	ft_printf("(group ");
	print_expr(expr->members.grouping.expr);
	ft_printf(")");
}

static void print_literal_bool(bool *value)
{
	if (*value)
		ft_printf("TRUE");
	else
		ft_printf("FALSE");
}

static void print_literal_number(double *value)
{
	printf("%.2f", *value);
	fflush(stdout);
}

static void print_literal_string(char *value)
{
	ft_printf("\"%s\"", value);
}