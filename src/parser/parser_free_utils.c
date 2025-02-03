/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 10:28:07 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 10:28:07 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_redi(t_redirect *red)
{
	t_redirect	*tmp;

	while (red)
	{
		tmp = red->next;
		free_tokens(red->tok_file);
		if (red->file)
		{
			free(red->file);
			red->file = NULL;
		}
		free(red);
		red = tmp;
	}
}

t_ast	*free_ast(t_ast *ast)
{
	if (!ast)
		return (NULL);
	free_ast(ast->left);
	free_ast(ast->right);
	free_redi(ast->redc);
	ast->redc = NULL;
	free_tokens(ast->tok_args);
	ast->tok_args = NULL;
	free_arr(ast->args);
	ast->args = NULL;
	free(ast);
	ast = NULL;
	return (NULL);
}
