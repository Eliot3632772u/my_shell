/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:05:46 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/25 02:10:45 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*pwd(char *str)
{
	static char	*pwd;
	t_env		*env;
	t_env		*pwd_node;

	if (!str)
		return (pwd);
	if (pwd)
		free(pwd);
	pwd = ft_strdup(str);
	env = get_envp(NULL);
	pwd_node = search_in_env(env, "PWD");
	if (pwd_node)
		set_env_value(ft_strdup("OLDPWD"), ft_strdup(pwd_node->value), 1);
	else
		set_env_value(ft_strdup("OLDPWD"), ft_strdup(""), 1);
	set_env_value(ft_strdup("PWD"), ft_strdup(pwd), 1);
	return (pwd);
}

int	ft_pwd(void)
{
	char	*current;

	current = pwd(NULL);
	if (current)
		ft_printf("%s\n", current);
	return (0);
}
