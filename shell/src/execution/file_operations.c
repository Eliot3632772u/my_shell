/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers_7.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 06:01:40 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/10 08:55:25 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_no_execute_perm(char **cmd)
{
	set_exit_status(126);
	print_err(cmd[0], 0);
	exit(126);
}

void	handle_file_not_found(char **cmd)
{
	set_exit_status(127);
	print_err(cmd[0], -5);
	exit(127);
}

void	handle_non_exec_path(char **cmd, char *try_path)
{
	free(try_path);
	set_exit_status(126);
	print_err(cmd[0], 0);
	exit(126);
}

void	handle_directory_case(char **cmd)
{
	set_exit_status(126);
	print_err(cmd[0], -69);
	exit(126);
}

int	execute_direct_path(char **cmd, t_env *env)
{
	struct stat	file_stat;
	char		**envp;

	if (stat(cmd[0], &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode) && !ft_strchr(cmd[0], '/'))
			handle_dir_error(cmd);
		if (!S_ISDIR(file_stat.st_mode) && !ft_strchr(cmd[0], '/'))
		{
			if (access(cmd[0], X_OK) == 0)
			{
				envp = consume_env(env);
				execve(cmd[0], cmd, envp);
				free_list(envp);
				return (-1);
			}
			handle_dir_error(cmd);
		}
	}
	return (0);
}
