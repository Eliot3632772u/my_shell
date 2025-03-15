/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:05:56 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/12 11:38:14 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	del_from_env(t_env **env, char *key)
{
	t_env	*node;

	if (!env || !*env)
		return ;
	node = search_in_env(*env, key);
	if (!node)
		return ;
	if (node == *env)
		*env = (*env)->next;
	else if (!node->next)
		node->prev->next = NULL;
	else
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	ft_lstdelone2(node, free);
}

int	ft_unset(int argc, char **args, t_env **env)
{
	int		i;

	if (!args || !env || !*env)
		return (1);
	i = 0;
	while (i < argc)
	{
		if (!args[i] || ft_strcmp(args[i], "") == 0)
		{
			i++;
			continue ;
		}
		del_from_env(env, args[i++]);
	}
	return (0);
}
