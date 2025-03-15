/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:49:37 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/15 17:12:31 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_token_after_heredoc(t_token *tok, \
	t_redirect *tree, t_str *s_ptr)
{
	tok->len = ft_strlen(tok->value);
	tree->mode = O_RDONLY;
	free_tok_lst(tok->nospace_next);
	tok->nospace_next = NULL;
	free(s_ptr->str);
	free(s_ptr);
	tok->to_expand = false;
}

int	patch_token(t_redirect *redc)
{
	t_token			*tok;
	t_str			*s_ptr;
	char			*unquoted;

	if (!redc)
		return (1);
	s_ptr = NULL;
	unquoted = get_processed_delimiter(redc, &s_ptr);
	tok = redc->file_tok;
	free(tok->value);
	redc->expand = is_expandable(redc->file_tok);// change this to be redc->heredoc_expand
	tok->value = handle_heredoc(unquoted);
	free(unquoted);
	if (!tok->value)
	{
		free(s_ptr->str);
		free(s_ptr);
		return (0);
	}
	handle_heredoc_signal();
	signal(SIGINT, sigint_handler);
	update_token_after_heredoc(tok, redc, s_ptr);
	return (1);
}
