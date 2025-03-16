/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:02:07 by irabhi            #+#    #+#             */
/*   Updated: 2025/03/16 11:15:33 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	add_redic(t_redirect **redic, t_token **tok)
{
	t_redirect	*tmp;

	if (!(*redic))
	{
		*redic = new_redic((*tok)->type);
		if (!(*redic))
			return (1);
		(*redic)->mode = get_redic_mode(*tok);
		free_tok_node(tok);
		(*redic)->file_tok = get_file_tokens(tok);
		return (0);
	}
	tmp = *redic;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_redic((*tok)->type);
	if (!tmp->next)
		return (1);
	tmp->next->prev = tmp;
	tmp->next->mode = get_redic_mode(*tok);
	free_tok_node(tok);
	tmp->next->file_tok = get_file_tokens(tok);
	return (0);
}

t_token	*get_file_tokens(t_token **tok)
{
	t_token	*tmp;

	if (!tok || !(*tok))
		return (NULL);
	tmp = (*tok);
	(*tok) = (*tok)->next;
	if (tmp->next)
		tmp->next->prev = NULL;
	tmp->next = NULL;
	return (tmp);
}

t_redirect	*new_redic(t_token_type type)
{
	t_redirect	*new;

	new = malloc(sizeof(t_redirect));
	if (!new)
		return (NULL);
	new->type = type;
	new->file_tok = NULL;
	new->next = NULL;
	new->mode = -1;
	new->expand = 0;
	new->prev = NULL;
	return (new);
}

void	add_arg(t_token **argv_tok, t_token **tok)
{
	t_token	*tmp;

	if (*argv_tok == NULL)
		*argv_tok = *tok;
	else
	{
		tmp = *argv_tok;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = *tok;
	}
	while ((*tok) && check_word((*tok)->type))
		(*tok) = (*tok)->next;
	if (*tok)
	{
		(*tok)->prev->next = NULL;
		(*tok)->prev = NULL;
	}
}

t_ast	*new_cmd(t_token *token)
{
	t_ast	*tmp;
	t_token	*list;

	tmp = malloc(sizeof(t_ast));
	if (!tmp)
		return (NULL);
	tmp->left = NULL;
	tmp->right = NULL;
	tmp->redc = NULL;
	tmp->type = P_EXEC;
	tmp->argv_tok = NULL;
	if (!token || !check_word(token->type))
		return (tmp);
	tmp->argv_tok = token;
	list = token;
	while (list->next && check_word(list->next->type))
		list = list->next;
	if (list->next)
		list->next->prev = NULL;
	list->next = NULL;
	return (tmp);
}
