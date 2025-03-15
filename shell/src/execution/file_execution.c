/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:59:17 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/13 13:01:29 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	check_if_executable(char **cmd, char *exec, char **paths, t_env *env)
{
	char	**envp;

	if (access(exec, X_OK) == 0)
	{
		envp = consume_env(env);
		execve(exec, cmd, envp);
		free_list(envp);
		free_list(paths);
	}
	return (false);
}

void	try_execute_file(char **cmd, t_env *env)
{
	char		**envp;
	struct stat	file_stat;

	if (stat(cmd[0], &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
			handle_directory_case(cmd);
		if (access(cmd[0], F_OK) == 0)
		{
			if (access(cmd[0], X_OK) == 0)
			{
				envp = consume_env(env);
				execve(cmd[0], cmd, envp);
				free_list(envp);
			}
			handle_no_execute_perm(cmd);
		}
	}
	handle_file_not_found(cmd);
}

void	try_local_execution(char **cmd, char *try_path, t_env *env)
{
	char	**envp;

	if (access(try_path, X_OK) == 0)
	{
		envp = consume_env(env);
		execve(try_path, cmd, envp);
		free_list(envp);
		free(try_path);
	}
	else
		handle_non_exec_path(cmd, try_path);
}
