/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 10:30:46 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 10:30:46 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tok_node(t_token **tok)
{
	t_token	*tmp;

	tmp = (*tok);
	(*tok) = (*tok)->next;
	if (*tok)
		(*tok)->prev = NULL;
	free(tmp->value);
	free(tmp);
}

int	add_redic(t_redirect **redic, t_token **tok, t_shell *shell)
{
	t_redirect	*tmp;

	if (!(*redic))
	{
		*redic = new_redic((*tok)->type, shell);
		if (!(*redic))
			return (1);
		free_tok_node(tok);
		(*redic)->tok_file = get_file_tokens(tok);
		return (0);
	}
	tmp = *redic;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_redic((*tok)->type, shell);
	if (!tmp->next)
		return (1);
	free_tok_node(tok);
	tmp->next->tok_file = get_file_tokens(tok);
	return (0);
}

void	add_arg(t_token **tok_args, t_token **tok)
{
	t_token	*tmp;

	if (*tok_args == NULL)
		*tok_args = *tok;
	else
	{
		tmp = *tok_args;
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

t_token	*get_file_tokens(t_token **tok)
{
	t_token	*tmp;

	tmp = (*tok);
	while ((*tok) && (*tok)->concate)
		(*tok) = (*tok)->next;
	(*tok) = (*tok)->next;
	if ((*tok))
	{
		(*tok)->prev->next = NULL;
		(*tok)->prev = NULL;
	}
	return (tmp);
}
