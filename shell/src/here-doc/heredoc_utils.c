/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:49:37 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 21:43:59 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_token_after_heredoc(t_token *tok, t_redirect *tree, t_str *s_ptr)
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
	t_token	*tok;
	t_str	*s_ptr;
	char	*unquoted;

	if (!redc)
		return (1);
	s_ptr = NULL;
	unquoted = get_processed_delimiter(redc, &s_ptr);
	tok = redc->file_tok;
	free(tok->value);
	redc->expand = is_expandable(redc->file_tok);
	tok->value = handle_heredoc(unquoted, redc->type);
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

char	*ft_mktmp(void)
{
	t_strbuilder	*sb;
	char			*name;
	int				fd;
	static int		counter;

	counter++;
	sb = stringbuilder();
	sb_append(sb, "/tmp/.Minishell_HEREDOC_");
	sb_append_int(sb, counter);
	name = ft_strdup(sb->str);
	sb_free(sb);
	fd = open(name, O_RDWR | O_CREAT, 0644);
	if (fd != -1)
	{
		unlink(name);
		close(fd);
		return (name);
	}
	free(name);
	return (NULL);
}

int	heredoc_limit(int mode)
{
	static int	limit = 0;
	int			tmp;

	if (mode == SET)
	{
		limit++;
	}
	else
	{
		tmp = limit;
		limit = 0;
		return (tmp);
	}
	return (-1);
}
