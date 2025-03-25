/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 02:46:46 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/25 02:47:21 by yrafai           ###   ########.fr       */
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

char	*get_processed_delimiter(t_redirect *redc, t_str **s_ptr)
{
	char	*processed;
	char	*result;

	expand_nosp_arg(redc->file_tok, s_ptr, 1);
	processed = process_delimiter_with_dollars((*s_ptr)->str);
	if (!processed)
		return (NULL);
	result = strip_quotes(processed);
	free(processed);
	return (result);
}

void	heredoc_limit_check(t_ast *ast)
{
	t_redirect	*redc;

	if (ast == NULL)
		return ;
	heredoc_limit_check(ast->left);
	heredoc_limit_check(ast->right);
	if (ast->redc)
	{
		redc = ast->redc;
		while (redc)
		{
			if (redc->type == HEREDOC)
				heredoc_limit(SET);
			redc = redc->next;
		}
	}
}
