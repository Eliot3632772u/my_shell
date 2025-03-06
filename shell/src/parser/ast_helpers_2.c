/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_helpers_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:31:42 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/04 00:49:52 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_heredocs_in_tokens(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type != NEW_LINE)
	{
		if (current->type == HEREDOC || current->type == HEREDOC_TAB)
			count++;
		current = current->next;
	}
	return (count);
}

static bool	handle_heredoc_limit(t_token **current)
{
	const bool	is_heredoc = (*current)->type == HEREDOC \
		|| (*current)->type == HEREDOC_TAB;

	if (is_heredoc && count_heredocs_in_tokens(*current) > 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2);
		set_exit_status(2);
		while (*current && (*current)->type != NEW_LINE)
			advance(current);
		if (*current)
			advance(current);
		return (true);
	}
	return (false);
}

static t_ast_cmd	*process_redirection(t_token **current, t_token *start)
{
	advance(current);
	if (!match(*current, (t_token_type[]){WORD, STR, DQSTR}, 3))
		return (NULL);
	return ((t_ast_cmd *)tok_to_redir(start));
}

t_ast_cmd	*redir_file(t_token **current)
{
	t_token	*start;

	if (!match(*current, (t_token_type[]){OUTPUT, APPEND, INPUT, \
		HEREDOC, HEREDOC_TAB}, 5))
		return (NULL);
	if (handle_heredoc_limit(current))
		return (NULL);
	start = *current;
	return (process_redirection(current, start));
}

bool	add_redir_node(t_ast_redir **lst, t_ast_cmd *node)
{
	t_ast_redir	*tmp;

	if (!lst || !node)
		return (false);
	if (!(*lst))
	{
		*lst = (t_ast_redir *)node;
		return (true);
	}
	tmp = *lst;
	while (tmp && tmp->cmd)
		tmp = (t_ast_redir *)(tmp)->cmd;
	tmp->cmd = node;
	return (true);
}
