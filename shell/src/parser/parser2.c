/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:00:21 by irabhi            #+#    #+#             */
/*   Updated: 2025/03/16 14:41:40 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*parse_redi(t_token **tok, int *err)
{
	t_ast	*ast;

	ast = parse_cmd2(tok, err);
	if (!ast || *err)
		return (ast);
	while (*tok && check_redir((*tok)->type))
	{
		if (!(*tok)->next || !check_word((*tok)->next->type))
		{
			if (ast->type != P_SUBSH)
				free_tok_node(tok);
			*err = 1;
			return (ast);
		}
		if (add_redic(&(ast->redc), tok))
			return (ast);
		if (*tok && check_word((*tok)->type))
			add_arg(&(ast->argv_tok), tok);
	}
	return (ast);
}

t_ast	*parse_pipe2(t_token **tok, int *err)
{
	t_ast	*ast;
	t_ast	*root;

	ast = parse_redi(tok, err);
	if (!ast || *err)
		return (ast);
	if ((*tok) && (*tok)->type == PIPE)
	{
		free_tok_node(tok);
		if (!(*tok) || check_cmd((*tok)->type))
		{
			*err = 1;
			return (ast);
		}
		root = new_ast(P_PIPE);
		if (!root)
			return (ast);
		root->left = ast;
		root->right = parse_logical(tok, err);
		return (root);
	}
	return (ast);
}

t_ast	*parse_logical(t_token **tok, int *err)
{
	t_ast		*ast;
	t_ast		*root;
	t_node_type	type;

	ast = parse_pipe2(tok, err);
	if (!ast || *err)
		return (ast);
	if ((*tok) && check_logical((*tok)->type))
	{
		type = P_AND;
		if ((*tok)->type == OR)
			type = P_OR;
		if (!(*tok)->next || (*tok)->next->type == PIPE || \
		check_logical((*tok)->next->type))
			return ((*err = 1), (ast));
		free_tok_node(tok);
		root = new_ast(type);
		if (root == NULL)
			return (ast);
		root->left = ast;
		root->right = parse_logical(tok, err);
		return (root);
	}
	return (ast);
}

t_ast	*unexpec_tok(t_token **tok, t_ast *ast)
{
	set_exit_status(258);
	ft_putstr_fd("Minishell: syntax error near unexpected token `", 2);
	if (*tok)
		ft_putstr_fd((*tok)->value, 2);
	else
		ft_putstr_fd("\\n", 2);
	write(2, "'\n", 2);
	if (ast)
		ft_heredoc(ast);
	cleanup_all_heredoc_files();
	free_ast2(ast);
	free_tok_lst(*tok);
	*tok = NULL;
	return (NULL);
}

t_ast	*parser2(t_token **tok)
{
	t_ast	*ast;
	int		err;

	err = 0;
	if (!(*tok))
		return (NULL);
	ast = parse_logical(tok, &err);
	if ((*tok) || err)
		return (unexpec_tok(tok, ast));
	return (ast);
}
