/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:04:50 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/03 23:04:51 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_cd(char *arg, t_env *env)
{
	t_env	*node;

	if (!arg || !*arg || ft_strcmp(arg, "~") == 0)
	{
		node = search_in_env(env, "HOME");
		if (!node)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
		return (change_directory(node->value));
	}
	else if (ft_strcmp(arg, "-") == 0)
	{
		node = search_in_env(env, "OLDPWD");
		if (!node)
		{
			printf("cd: OLDPWD not set\n");
			return (1);
		}
		printf("%s\n", node->value);
		return (change_directory(node->value));
	}
	else
		return (change_directory(arg));
}
