/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:52:02 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/04 00:52:10 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	parser(t_token *tokens, t_ast_cmd **tree)
{
	t_token	*current;

	*tree = NULL;
	if (!tokens)
		return (false);
	current = tokens;
	if (current->type == NEW_LINE)
		return (free_tok_lst(tokens), true);
	*tree = parse_cmd(&current);
	if (!(*tree))
	{
		if (get_exit_status() == 2)
			return (free_tok_lst(tokens), false);
		syntax_error(current->value);
		return (free_tok_lst(tokens), false);
	}
	if (current->type != NEW_LINE)
	{
		syntax_error(current->value);
		return (free_tok_lst(tokens), false);
	}
	free_tok_lst(tokens);
	return (ft_heredoc(*tree));
}
