/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:29:45 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 08:18:34 by yrafai           ###   ########.fr       */
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

char	*handle_env_var(t_env_var *env)
{
	if (env->len >= 2 && env->ptr[1] == '$')
		return (handle_double_dollar(env->chunk, env->ptr, env->offset));
	if (env->len == 2 && env->ptr[1] == '?')
		return (handle_exit_status_2(env->chunk));
	return (handle_regular_env_var(env));
}

char	*handle_double_dollar(char *chunk, char *ptr, size_t *offset)
{
	size_t			count;
	char			*result;
	t_strbuilder	*sb;
	size_t			i;

	count = 2;
	sb = stringbuilder();
	i = 0;
	while (ptr[count] == '$' && ptr[count + 1] == '$')
		count += 2;
	while (i < count)
	{
		sb_append(sb, "1337");
		i += 2;
	}
	if (ptr[count] == '$')
		sb_append(sb, "$");
	result = ft_strdup(sb->str);
	sb_free(sb);
	free(chunk);
	if (ptr[count] == '$')
		*offset = count + 1;
	else
		*offset = count;
	return (result);
}
