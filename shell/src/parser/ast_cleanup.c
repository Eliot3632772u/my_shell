/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_cleanup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:47:09 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/15 18:05:18 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_ast(t_ast_cmd *tree)
{
	if (!tree)
		return ;
	if (tree->type == P_EXEC)
		free_tok_lst(((t_ast_exec *)tree)->argv_tok);
	else if (tree->type == P_REDIR)
	{
		free_tok_lst(((t_ast_redir *)tree)->file_tok);
		free_ast(((t_ast_redir *)tree)->cmd);
	}
	else if (tree->type == P_SUBSH)
		free_ast(((t_ast_subsh *)tree)->cmd);
	else
	{
		free_ast(((t_ast_binary *)tree)->left);
		free_ast(((t_ast_binary *)tree)->right);
	}
	free(tree);
}

void	free_redir(t_ast_cmd *sub_sh, t_ast_redir *redir_lst, t_token *exe_lst)
{
	free_ast(sub_sh);
	free_ast((t_ast_cmd *)redir_lst);
	free_tok_lst(exe_lst);
}

t_ast	*free_ast2(t_ast *ast)
{
	if (!ast)
		return (NULL);
	free_ast2(ast->left);
	free_ast2(ast->right);
	free_redi(ast->redc);
	ast->redc = NULL;
	free_tok_lst(ast->argv_tok);
	ast->argv_tok = NULL;
	free(ast);
	ast = NULL;
	return (NULL);
}

void	free_redi(t_redirect *red)
{
	t_redirect	*tmp;

	while (red)
	{
		tmp = red->next;
		free_tok_lst(red->file_tok);
		free(red);
		red = tmp;
	}
}
