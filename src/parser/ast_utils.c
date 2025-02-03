/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 10:02:16 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 10:02:16 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*new_cmd(t_token *token, t_shell *shell)
{
	t_ast	*tmp;
	t_token	*list;

	tmp = malloc(sizeof(t_ast));
	if (!tmp)
	{
		shell->error = ERR_MEMORY;
		return (NULL);
	}
	tmp->left = NULL;
	tmp->right = NULL;
	tmp->redc = NULL;
	tmp->args = NULL;
	tmp->type = ast_cmd;
	tmp->tok_args = NULL;
	if (!token || !check_word(token->type))
		return (tmp);
	tmp->tok_args = token;
	list = token;
	while (list->next && check_word(list->next->type))
		list = list->next;
	if (list->next)
		list->next->prev = NULL;
	list->next = NULL;
	return (tmp);
}

t_redirect	*new_redic(t_token_type type, t_shell *shell)
{
	t_redirect	*new;

	new = malloc(sizeof(t_redirect));
	if (!new)
	{
		shell->error = ERR_MEMORY;
		return (NULL);
	}
	new->type = type;
	new->file = NULL;
	new->tok_file = NULL;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_ast	*new_ast(t_ast_type type, t_shell *shell)
{
	t_ast	*new;

	new = malloc(sizeof(t_ast));
	if (!new)
	{
		shell->error = ERR_MEMORY;
		return (NULL);
	}
	new->args = NULL;
	new->type = type;
	new->redc = NULL;
	new->tok_args = NULL;
	new->left = NULL;
	new->right = NULL;
	return (new);
}
