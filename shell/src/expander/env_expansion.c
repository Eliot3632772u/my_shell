/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:15:58 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/15 10:18:06 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_dollar_quoted_str(const char *str)
{
	return (str[0] == '$' && (str[1] == '"' || str[1] == '\''));
}

char	*handle_dollar_quoted(char *str)
{
	char	quote;
	char	*content;
	char	*expanded;

	quote = str[1];
	content = ft_substr(str, 2, ft_strlen(str) - 3);
	if (!content)
		return (NULL);
	if (quote == '"' && ft_strchr(content, '$'))
	{
		expanded = expand_env(content, true, false);
		return (expanded);
	}
	return (content);
}

bool	process_env_chunks(t_strbuilder *sb, char *to_expand, \
	bool in_quote, bool ignore_env)
{
	char			*ptr;
	char			*chunk;
	size_t			offset;
	t_chunk_info	info;

	ptr = to_expand;
	while (ptr && *ptr)
	{
		info.sb = sb;
		info.chunk = &chunk;
		info.ptr = ptr;
		info.in_quote = in_quote;
		info.ignore_env = ignore_env;
		info.offset = &offset;
		if (!handle_chunk(&info))
		{
			ptr += offset;
			continue ;
		}
		if (!chunk)
			return (false);
		sb_append_free(sb, chunk);
		ptr += offset;
	}
	return (true);
}

char	*expand_env(char *to_expand, bool in_quote, bool ignore_env)
{
	t_strbuilder	*sb;
	char			*result;

	if (!to_expand)
		return (NULL);
	sb = stringbuilder();
	if (!sb)
	{
		free(to_expand);
		return (NULL);
	}
	if (!process_env_chunks(sb, to_expand, in_quote, ignore_env))
	{
		sb_free(sb);
		free(to_expand);
		return (NULL);
	}
	result = ft_strdup(sb->str);
	sb_free(sb);
	free(to_expand);
	return (result);
}

bool	expand_environment_variable(t_chunk_info *info, size_t len)
{
	t_env_var	env;
	char		*expanded;

	env.chunk = *(info->chunk);
	env.ptr = info->ptr;
	env.len = len;
	env.in_quote = info->in_quote;
	env.offset = info->offset;
	expanded = handle_env_var(&env);
	*(info->chunk) = expanded;
	if (!*(info->chunk))
		return (false);
	if (!info->in_quote && info->ptr[len] == '\0' && expanded
		&& (ft_strchr(*(info->chunk), ' ') || ft_strchr(*(info->chunk), '\t')))
	{
		sb_append(info->sb, *(info->chunk));
		free(*(info->chunk));
		*(info->chunk) = NULL;
		return (false);
	}
	return (true);
}
