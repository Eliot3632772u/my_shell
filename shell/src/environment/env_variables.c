/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:03:03 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/03 23:03:55 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	create_env(char **envp)
{
	t_env	**env;
	t_env	*prev;
	t_env	*ptr;

	env = get_envp_internal(NULL);
	prev = NULL;
	if (!envp || !*envp)
	{
		set_env_value(ft_strdup("PATH"),
			ft_strdup(DEFAULT_PATH), 1);
		set_env_value(ft_strdup("SHLVL"),
			ft_strdup("1"), 1);
		handle_shlvl();
		return ;
	}
	while (*envp)
	{
		ptr = ft_lstnew2(get_key(*envp), get_value(*envp),
				!!ft_strchr(*envp, '='), prev);
		ft_lstadd_back2(env, ptr);
		prev = ptr;
		envp++;
	}
	handle_shlvl();
}

void	log_last_command(char *value)
{
	if (!value)
		return ;
	set_env_value(ft_strdup("_"), ft_strdup(value), 1);
}

char	*get_key(char *line)
{
	size_t	index;
	char	*equal_sign;

	if (!line || !*line)
		return (NULL);
	equal_sign = ft_strchr(line, '=');
	if (equal_sign)
	{
		index = (size_t)equal_sign - (size_t)line;
		if (is_concate(line))
			index--;
		return (ft_substr(line, 0, index));
	}
	return (ft_strdup(line));
}

char	*get_value(char *line)
{
	char	*equal_sign;

	if (!line || !*line)
		return (NULL);
	equal_sign = ft_strchr(line, '=');
	if (!equal_sign)
		return (ft_strdup(""));
	return (ft_substr(equal_sign, 1, ft_strlen(equal_sign + 1)));
}

void	set_env_value(char *key, char *value, int equal_sign)
{
	t_env	**env;
	t_env	*node;

	if (!key || !value)
		return ;
	env = get_envp_internal(NULL);
	node = search_in_env(*env, key);
	if (!node)
		append_to_env(env, key, value, equal_sign);
	else
	{
		edit_env(node, value, equal_sign);
		free(key);
	}
}
