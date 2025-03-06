/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:52:02 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/06 15:42:23 by yrafai           ###   ########.fr       */
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
	if (!current)
		return (false);
	if (current->type == NEW_LINE)
		return (free_tok_lst(tokens), true);
	*tree = parse_cmd(&current);
	if (!current || !(*tree) || current->type != NEW_LINE)
	{
		if (current)
			syntax_error(current->value);
		return (free_tok_lst(tokens), false);
	}
	free_tok_lst(tokens);
	return (ft_heredoc(*tree));
}
