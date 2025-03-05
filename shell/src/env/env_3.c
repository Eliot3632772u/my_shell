/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:04:19 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/03 23:04:20 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	append_to_env(t_env **env, char *key, char *value, int equal_sign)
{
	t_env	*prev;
	t_env	*node;

	if (!env || !value)
		return ;
	prev = NULL;
	if (*env)
		prev = ft_lstlast2(*env);
	node = ft_lstnew2(key, value, equal_sign, prev);
	ft_lstadd_back2(env, node);
}

void	edit_env(t_env *node, char *value, int equal_sign)
{
	if (!node || !value)
		return ;
	free(node->value);
	node->value = value;
	node->equal_sign = equal_sign;
}

void	concate_env(char *key, char *value)
{
	t_env	*node;
	char	*old_value;

	if (!key)
		return ;
	node = search_in_env(get_envp(NULL), key);
	if (node)
	{
		free(key);
		old_value = node->value;
		node->value = ft_strjoin(old_value, value);
		node->equal_sign = 1;
		free(old_value);
		free(value);
	}
	else
	{
		node = ft_lstnew2(key, value, 1, ft_lstlast2(get_envp(NULL)));
		ft_lstadd_back2(get_envp_internal(NULL), node);
	}
}

void	handle_shlvl(void)
{
	t_env	*shlvl_node;
	int		level;
	char	*tmp;

	shlvl_node = search_in_env(get_envp(NULL), "SHLVL");
	if (!shlvl_node || !shlvl_node->value)
		level = 1;
	else
	{
		level = ft_atoi(shlvl_node->value);
		if (level < 0)
			level = 0;
		level++;
		if (level >= 1000)
		{
			ft_putendl_fd(SHLVL_WARN, 2);
			level = 1;
		}
	}
	tmp = ft_itoa(level);
	set_env_value(ft_strdup("SHLVL"), tmp, 1);
}

char	*get_env_value(char *var)
{
	t_env	*node;

	if (!var)
		return (NULL);
	node = search_in_env(get_envp(NULL), var + 1);
	free(var);
	if (!node || !node->value)
		return (NULL);
	return (ft_strdup(node->value));
}
