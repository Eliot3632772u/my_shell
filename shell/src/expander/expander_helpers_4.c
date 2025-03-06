/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helpers_4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 08:25:34 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/06 08:26:11 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	expand_environment_variable(t_chunk_info *info, size_t len)
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
	if (info->ptr[len] == '\0' && (ft_chr(*(info->chunk), ' ')
			|| ft_chr(*(info->chunk), '\t')))
	{
		sb_append(info->sb, *(info->chunk));
		free(*(info->chunk));
		*(info->chunk) = NULL;
		return (false);
	}
	return (true);
}

bool	handle_chunk(t_chunk_info *info)
{
	size_t		len;

	len = get_chunk_len(info->ptr, "$");
	*(info->chunk) = ft_substr(info->ptr, 0, len);
	*(info->offset) = len;
	if (!info->ignore_env && *(info->ptr) == '$' && len > 1)
	{
		if (!expand_environment_variable(info, len))
			return (false);
	}
	return (true);
}
