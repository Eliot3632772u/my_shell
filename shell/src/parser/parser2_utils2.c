/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 08:15:44 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 08:16:00 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*parse_sub(t_token **tok, int *err)
{
	t_ast	*subshell;

	subshell = NULL;
	free_tok_node(tok);
	if (!(*tok) || check_cmd((*tok)->type))
		return (NULL);
	if ((*tok))
	{
		subshell = new_ast(P_SUBSH);
		if (subshell == NULL)
			return (NULL);
		subshell->left = parse_logical(tok, err);
	}
	if (!(*tok) || (*tok)->type != RPREN || *err)
		return ((*err = 1), subshell);
	free_tok_node(tok);
	return (subshell);
}

t_ast	*parse_cmd2(t_token **tok, int *err)
{
	t_token	*tmp;
	t_ast	*ast;

	if ((*tok) && (*tok)->type == LPREN)
		return (parse_sub(tok, err));
	if (!(*tok) || check_cmd((*tok)->type))
		return (NULL);
	tmp = *tok;
	while (*tok && check_word((*tok)->type))
		(*tok) = (*tok)->next;
	ast = new_cmd(tmp);
	return (ast);
}

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

t_ast	*new_ast(t_node_type type)
{
	t_ast	*new;

	new = malloc(sizeof(t_ast));
	if (!new)
		return (NULL);
	new->type = type;
	new->redc = NULL;
	new->argv_tok = NULL;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

int	get_redic_mode(t_token *redic)
{
	if (redic->type == APPEND)
		return (O_CREAT | O_WRONLY | O_APPEND | O_TRUNC);
	else if (redic->type == OUTPUT)
		return (O_CREAT | O_WRONLY | O_TRUNC);
	else if (redic->type == INPUT)
		return (O_RDONLY);
	return (-1);
}
