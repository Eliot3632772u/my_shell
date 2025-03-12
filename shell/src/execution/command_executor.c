/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:12:45 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/12 08:06:19 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*find_path(t_env *envp)
{
	t_env	*node;

	node = search_in_env(envp, "PATH");
	if (!node)
		return (NULL);
	return (node->value);
}

int	is_dir(char *str)
{
	DIR		*ptr;
	int		res;

	if (!str)
		return (0);
	ptr = opendir(str);
	res = !!ptr;
	if (ptr)
		closedir(ptr);
	return (res);
}

int	check_dir(char **cmd)
{
	if (!ft_strcmp(cmd[0], "."))
	{
		set_exit_status(2);
		print_err(cmd[0], -69);
		exit(2);
	}
	if (!ft_strcmp(cmd[0], ".."))
	{
		set_exit_status(127);
		print_err(cmd[0], -1);
		exit(127);
	}
	if (is_dir(cmd[0]))
	{
		if (ft_strchr(cmd[0], '/'))
		{
			set_exit_status(126);
			print_err(cmd[0], -69);
			exit(126);
		}
		set_exit_status(127);
		print_err(cmd[0], -1);
		exit(127);
	}
	return (0);
}

int	execute_cmd(char **cmd, t_env *env)
{
	char			**paths;
	char			**tmp;
	t_strbuilder	*sb;

	if (try_direct_path(cmd, env) == -1)
		return (-1);
	if (!find_path(env))
		return (-1);
	paths = ft_split(find_path(env), ':');
	tmp = paths;
	if (!paths)
		return (-2);
	while (*tmp)
	{
		sb = stringbuilder();
		sb_append(sb_append(sb_append_free(sb, *tmp), "/"), cmd[0]);
		*tmp = sb->str;
		free(sb);
		if (check_if_executable(cmd, *tmp, paths, env))
			return (-1);
		tmp++;
	}
	return (free_list(paths), -1);
}
