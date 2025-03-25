/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:29:45 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/25 07:48:04 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*process_expanded_env_value(t_env_var *env)
{
	if (!env->in_quote)
	{
		*(env->offset) = env->len;
		if (env->ptr[env->len] == '='
			|| (env->ptr[env->len] == '"' && env->ptr[env->len + 1] == '='))
			return (env->chunk);
		if (env->ptr[env->len] != '\0'
			&& env->ptr[env->len] != ' ' && env->ptr[env->len] != '\t')
		{
			if (ft_strchr(env->chunk, ' ') || ft_strchr(env->chunk, '\t'))
				ghost_char(env->chunk);
			return (env->chunk);
		}
	}
	return (env->chunk);
}

char	*handle_regular_env_var(t_env_var *env)
{
	char	*env_value;

	env_value = get_env_value(env->chunk);
	if (!env_value)
	{
		*(env->offset) = env->len;
		env_value = ft_strdup("");
		if (!env_value)
			return (NULL);
		return (env_value);
	}
	env->chunk = env_value;
	return (process_expanded_env_value(env));
}

static char	*expand_remaining_dollar(char *ptr, size_t count, t_strbuilder *sb)
{
	char	*remainder;
	char	*expanded;

	remainder = NULL;
	expanded = NULL;
	if (ptr[count] == '$' && ptr[count + 1])
	{
		remainder = ft_strdup(ptr + count);
		if (remainder)
		{
			expanded = expand_env(remainder, false, false);
			if (expanded)
				sb_append(sb, expanded);
			free(expanded);
		}
	}
	return (remainder);
}

size_t	count_double_dollar_pairs(char *ptr, size_t *count)
{
	size_t	pairs;

	*count = 2;
	pairs = 1;
	while (ptr[*count] == '$' && ptr[*count + 1] == '$')
	{
		*count += 2;
		pairs++;
	}
	return (pairs);
}

char	*handle_double_dollar(char *chunk, char *ptr, size_t *offset)
{
	size_t			pairs;
	size_t			count;
	t_strbuilder	*sb;
	char			*result;
	char			*remainder;

	if (!ptr)
		return (NULL);
	sb = stringbuilder();
	if (!sb)
		return (NULL);
	pairs = count_double_dollar_pairs(ptr, &count);
	while (pairs--)
		sb_append(sb, "1337");
	remainder = expand_remaining_dollar(ptr, count, sb);
	if (remainder)
		*offset = ft_strlen(ptr);
	else
		*offset = count;
	result = ft_strdup(sb->str);
	sb_free(sb);
	free(chunk);
	return (result);
}
