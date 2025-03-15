/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:49:37 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/14 13:03:44 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_token_after_heredoc(t_token *tok, \
	t_ast_redir *tree, t_str *s_ptr)
{
	tok->len = ft_strlen(tok->value);
	tree->direction = INPUT;
	free_tok_lst(tok->nospace_next);
	tok->nospace_next = NULL;
	free(s_ptr->str);
	free(s_ptr);
	tok->to_expand = false;
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
