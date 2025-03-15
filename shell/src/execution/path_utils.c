/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:40:57 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/13 13:11:52 by yrafai           ###   ########.fr       */
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

int	check_direct_path(char *cmd)
{
	struct stat	st;

	if (!ft_strchr(cmd, '/'))
		return (-1);
	if (stat(cmd, &st) == -1)
	{
		set_exit_status(127);
		print_err(cmd, -5);
		exit(127);
	}
	if (S_ISDIR(st.st_mode))
	{
		set_exit_status(126);
		print_err(cmd, -69);
		exit(126);
	}
	return (0);
}

int	try_direct_path(char **cmd, t_env *env)
{
	if (check_direct_path(cmd[0]) == -1)
		return (-1);
	if (access(cmd[0], F_OK) == 0)
	{
		if (access(cmd[0], X_OK) == 0)
			return (execute_direct_cmd(cmd, env));
		set_exit_status(126);
		print_err(cmd[0], -7);
		exit(126);
	}
	set_exit_status(127);
	print_err(cmd[0], -5);
	exit(127);
}

bool	check_absolute_path(char **cmd, t_env *env)
{
	if (ft_strchr(cmd[0], '/'))
	{
		try_execute_file(cmd, env);
		return (true);
	}
	return (false);
}

int	try_local_path(char **cmd, t_env *env)
{
	struct stat	st;
	char		*local_cmd;
	char		**envp;

	local_cmd = ft_strjoin("./", cmd[0]);
	if (!local_cmd)
		return (-1);
	if (stat(cmd[0], &st) == 0 && access(cmd[0], X_OK) == 0)
	{
		envp = consume_env(env);
		execve(cmd[0], cmd, envp);
		free_list(envp);
		free(local_cmd);
		return (0);
	}
	free(local_cmd);
	return (-1);
}
