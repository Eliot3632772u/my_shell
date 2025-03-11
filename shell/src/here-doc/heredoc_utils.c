/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 11:15:35 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/04 11:16:32 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	init_heredoc(char *delim, char **tmp_file, int *fd)
{
	if (!delim)
		return (0);
	*tmp_file = ft_mktmp();
	*fd = open(*tmp_file, O_RDWR | O_CREAT | O_EXCL, 0640);
	if (*fd < 0)
	{
		print_err(*tmp_file, 0);
		set_exit_status(1);
		return (0);
	}
	return (1);
}

bool	is_expandable(t_token *tok)
{
	t_token	*ptr;

	if (!tok)
		return (1);
	ptr = tok;
	while (ptr)
	{
		if (ptr->type == STR)
			return (0);
		if (ptr->type == DQSTR)
			return (1);
		ptr = ptr->nospace_next;
	}
	return (1);
}

int	patch_token(t_ast_redir *tree)
{
	t_token			*tok;
	t_str			*s_ptr;
	char			*unquoted;
	t_heredoc_opts	opts;

	if (!tree)
		return (1);
	s_ptr = NULL;
	unquoted = get_processed_delimiter(tree, &s_ptr);
	tok = tree->file_tok;
	free(tok->value);
	opts.expandable = is_expandable(tree->file_tok);
	opts.strip_tabs = (tree->direction == HEREDOC_TAB);
	tok->value = handle_heredoc(unquoted, opts);
	free(unquoted);
	if (!tok->value)
	{
		free(s_ptr->str);
		free(s_ptr);
		return (0);
	}
	handle_heredoc_signal();
	signal(SIGINT, sigint_handler);
	update_token_after_heredoc(tok, tree, s_ptr);
	return (1);
}
