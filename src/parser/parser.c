/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 10:37:51 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 10:37:51 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*parse_sub(t_token **tok, t_shell *shell)
{
	t_ast	*subshell;

	subshell = NULL;
	if (!(*tok)->next || check_cmd((*tok)->next->type))
		return (NULL);
	free_tok_node(tok);
	if ((*tok) && (*tok)->type != C_P)
	{
		subshell = new_ast(ast_sub, shell);
		if (subshell == NULL)
			return (NULL);
		subshell->left = parse_logical(tok, shell);
	}
	if (!(*tok) || (*tok)->type != C_P)
	{
		shell->error = UNEXPECTED_TOKEN;
		return (subshell);
	}
	free_tok_node(tok);
	if (*tok && !check_logical((*tok)->type) && (*tok)->type != PIPE)
	{
		shell->error = UNEXPECTED_TOKEN;
		return (subshell);
	}
	return (subshell);
}

t_ast	*parse_cmd(t_token **tok, t_shell *shell)
{
	t_token	*tmp;
	t_ast	*ast;

	if ((*tok) && (*tok)->type == O_P)
		return (parse_sub(tok, shell));
	if (!(*tok) || check_cmd((*tok)->type))
		return (NULL);
	tmp = *tok;
	while (*tok && check_word((*tok)->type))
		(*tok) = (*tok)->next;
	ast = new_cmd(tmp, shell);
	return (ast);
}

t_ast	*parse_redi(t_token **tok, t_shell *shell)
{
	t_ast	*ast;

	ast = parse_cmd(tok, shell);
	if (!ast || shell->error)
		return (ast);
	while (*tok && check_redir((*tok)->type))
	{
		if (!(*tok)->next || !check_word((*tok)->next->type))
		{
			free_tok_node(tok);
			shell->error = UNEXPECTED_TOKEN;
			return (ast);
		}
		if (add_redic(&(ast->redc), tok, shell))
			return (ast);
		if (*tok && check_word((*tok)->type))
			add_arg(&(ast->tok_args), tok);
	}
	return (ast);
}

t_ast	*parse_pipe(t_token **tok, t_shell *shell)
{
	t_ast	*ast;
	t_ast	*root;

	ast = parse_redi(tok, shell);
	if (!ast || shell->error)
		return (ast);
	if ((*tok) && (*tok)->type == PIPE)
	{
		free_tok_node(tok);
		if (!(*tok) || (*tok)->type == PIPE || \
		check_logical((*tok)->type))
		{
			shell->error = UNEXPECTED_TOKEN;
			return (ast);
		}
		root = new_ast(ast_pip, shell);
		if (!root)
			return (ast);
		root->left = ast;
		root->right = parse_logical(tok, shell);
		return (root);
	}
	return (ast);
}

t_ast	*parse_logical(t_token **tok, t_shell *shell)
{
	t_ast		*ast;
	t_ast		*root;
	t_ast_type	type;

	ast = parse_pipe(tok, shell);
	if (!ast || shell->error)
		return (ast);
	if ((*tok) && check_logical((*tok)->type))
	{
		type = ast_and;
		if ((*tok)->type == T_OR)
			type = ast_or;
		if (!(*tok)->next || (*tok)->next->type == PIPE || \
		check_logical((*tok)->next->type))
			return ((shell->error = UNEXPECTED_TOKEN), (ast));
		free_tok_node(tok);
		root = new_ast(type, shell);
		if (root == NULL)
			return (ast);
		root->left = ast;
		root->right = parse_logical(tok, shell);
		return (root);
	}
	return (ast);
}
