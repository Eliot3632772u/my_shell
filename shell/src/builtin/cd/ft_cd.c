/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:04:50 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/13 04:07:37 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	handle_home_directory(t_env *env)
{
	t_env	*node;

	node = search_in_env(env, "HOME");
	if (!node)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return (1);
	}
	return (change_directory(node->value));
}

static int	handle_previous_directory(t_env *env)
{
	t_env	*node;

	node = search_in_env(env, "OLDPWD");
	if (!node)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		return (1);
	}
	ft_putstr_fd(node->value, 1);
	ft_putstr_fd("\n", 1);
	return (change_directory(node->value));
}

int	ft_cd(char *arg, t_env *env, int use_dash_dash)
{
	if (!arg || !*arg || ft_strcmp(arg, "~") == 0 || ft_strcmp(arg, "--") == 0)
		return (handle_home_directory(env));
	if (ft_strcmp(arg, "-") == 0)
		return (handle_previous_directory(env));
	if (arg[0] == '-' && !use_dash_dash && arg[1] != '\0')
	{
		ft_putstr_fd("cd: ", 2);
		ft_putchar_fd(arg[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		return (1);
	}
	if (ft_strchr(arg, ' '))
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	return (change_directory(arg));
}
