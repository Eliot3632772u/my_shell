/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers_4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:16:53 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/03 23:53:31 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_if_forked(bool forked)
{
	if (forked)
		exit(get_exit_status());
	return ;
}

void	wait_and_exit_status(int pid)
{
	int	exit_status;

	waitpid(pid, &exit_status, 0);
	set_exit_status(WEXITSTATUS(exit_status));
	if (WIFSIGNALED(exit_status) == 1)
	{
		if (WTERMSIG(exit_status) == 3)
			ft_putendl_fd("Quit (core dumped)", 2);
		set_exit_status(128 + exit_status);
	}
}

static int	open_redir_file(t_ast_redir *tree, char *file_name, bool forked)
{
	int	fd_to_dup;

	fd_to_dup = open(file_name, tree->mode, 0644);
	if (fd_to_dup < 0)
	{
		print_err(file_name, 0);
		set_exit_status(1);
		free(file_name);
		exit_if_forked(forked);
		return (-1);
	}
	free(file_name);
	return (fd_to_dup);
}

void	exec_redir(t_ast_redir *tree, bool forked)
{
	int		fd_to_dup;
	int		fd_backup;
	char	*file_name;

	file_name = check_file_tok(tree->file_tok);
	if (!file_name)
		return (exit_if_forked(forked));
	fd_to_dup = open_redir_file(tree, file_name, forked);
	if (fd_to_dup < 0)
		return ;
	fd_backup = dup(tree->fd);
	if (fd_to_dup != tree->fd)
	{
		dup2(fd_to_dup, tree->fd);
		close(fd_to_dup);
	}
	set_exit_status(0);
	executor(tree->cmd, forked);
	if (fd_backup != tree->fd)
	{
		dup2(fd_backup, tree->fd);
		close(fd_backup);
	}
}

void	exec_subsh(t_ast_subsh *tree, bool forked)
{
	pid_t	pid;
	int		exit_status;

	pid = ft_fork();
	if (!pid)
		executor(tree->cmd, true);
	waitpid(pid, &exit_status, 0);
	set_exit_status(WEXITSTATUS(exit_status));
	if (forked)
		exit(get_exit_status());
}
