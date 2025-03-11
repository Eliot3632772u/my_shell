/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 11:13:44 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/04 11:34:49 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_empty_quoted_string(char *str)
{
	return (str && str[0] && str[1] && !str[2]
		&& ((str[0] == '"' && str[1] == '"')
			|| (str[0] == '\'' && str[1] == '\'')));
}

char	*strip_quotes(char *str)
{
	int		len;
	char	*result;

	if (!str || !*str)
		return (ft_strdup(""));
	if (is_empty_quoted_string(str))
		return (ft_strdup(""));
	len = ft_strlen(str);
	if (len >= 2 && (str[0] == '"' || str[0] == '\'')
		&& str[0] == str[len - 1])
	{
		result = ft_substr(str, 1, len - 2);
		return (result);
	}
	return (ft_strdup(str));
}

char	*get_processed_delimiter(t_ast_redir *tree, t_str **s_ptr)
{
	expand_nosp_arg(tree->file_tok, s_ptr, 1);
	return (strip_quotes((*s_ptr)->str));
}

void	handle_heredoc_signal(void)
{
	int	fd;

	if (g_last_signal != 420)
		return ;
	fd = open("/dev/tty", O_RDONLY);
	if (fd == -1)
		return ;
	dup2(fd, STDIN_FILENO);
	close(fd);
}

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
