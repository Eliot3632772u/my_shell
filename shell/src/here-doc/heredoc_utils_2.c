/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:02:32 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/14 13:03:41 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_expandable(t_token *tok)
{
	t_token	*ptr;

	if (!tok)
		return (true);
	ptr = tok;
	while (ptr)
	{
		if (ptr->type == STR || ptr->type == DQSTR)
			return (false);
		ptr = ptr->nospace_next;
	}
	return (true);
}

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
