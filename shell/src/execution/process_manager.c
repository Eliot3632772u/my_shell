/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers_4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:16:53 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/11 08:39:23 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_if_forked(bool forked)
{
	if (forked)
		exit(get_exit_status());
}

void	wait_and_exit_status(int pid)
{
	int	exit_status;

	waitpid(pid, &exit_status, 0);
	if (WIFEXITED(exit_status))
		set_exit_status(WEXITSTATUS(exit_status));
	else if (WIFSIGNALED(exit_status))
	{
		if (WTERMSIG(exit_status) == SIGQUIT)
		{
			ft_putendl_fd("Quit (core dumped)", 2);
			set_exit_status(131);
		}
		else if (WTERMSIG(exit_status) == SIGINT)
		{
			ft_putendl_fd("", 2);
			set_exit_status(130);
		}
	}
}

int	open_redir_file(t_ast_redir *tree, char *file_name, bool forked)
{
	int	fd_to_dup;

	fd_to_dup = open(file_name, tree->mode, 0644);
	if (fd_to_dup < 0)
	{
		print_err(file_name, 0);
		set_exit_status(1);
		free(file_name);
		if (forked && tree->direction != INPUT)
			exit(1);
		return (-1);
	}
	free(file_name);
	return (fd_to_dup);
}

void	exec_redir(t_ast_redir *tree, bool forked)
{
	if (!forked)
		handle_nonforked_redir(tree);
	else
		handle_forked_redir(tree);
}

void	exec_subsh(t_ast_subsh *tree, bool forked)
{
	pid_t	pid;
	int		exit_status;

	pid = ft_fork_wrapper(NULL, 0);
	if (!pid)
		executor(tree->cmd, true);
	waitpid(pid, &exit_status, 0);
	set_exit_status(WEXITSTATUS(exit_status));
	if (forked)
		exit(get_exit_status());
}
