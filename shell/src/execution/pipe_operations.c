/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers_6.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 05:50:00 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/10 06:02:23 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_dir_error(char **cmd)
{
	set_exit_status(127);
	print_err(cmd[0], -1);
	exit(127);
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
