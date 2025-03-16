/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_logic_exec.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:58:14 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 21:09:25 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_dups(t_ast *sub_tree, int *fd, int fd_num)
{
	reset_default_sig_handlers();
	signal(SIGQUIT, SIG_DFL);
	close(fd[!fd_num]);
	if (dup2(fd[fd_num], fd_num) == -1)
		(print_err("dup", 0), exit(-1));
	close(fd[fd_num]);
	executor(sub_tree, true);
	exit(get_exit_status());
}

void	exec_pipe(t_ast *tree, bool forked)
{
	int		fd[2];
	pid_t	pids[2];
	int		exit_status;
	int		termsig;

	pids[0] = 0;
	pids[1] = 0;
	if (pipe(fd) == -1)
		(perror("pipe"), exit(1));
	pids[0] = ft_fork();
	if (!pids[0])
		handle_dups(tree->left, fd, STDOUT_FILENO);
	pids[1] = ft_fork();
	if (!pids[1])
		handle_dups(tree->right, fd, STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	waitpid(pids[1], &exit_status, 0);
	waitpid(pids[0], NULL, 0);
	termsig = WTERMSIG(exit_status);
	if (WIFSIGNALED(exit_status) && termsig == SIGQUIT)
		ft_printf("Quit (core dumped)\n");
	set_exit_status(WEXITSTATUS(exit_status));
	if (forked)
		exit(get_exit_status());
}

void	exec_or(t_ast *tree, bool forked)
{
	executor(tree->left, false);
	if (get_exit_status())
		executor(tree->right, false);
	if (forked)
		exit(get_exit_status());
}

void	exec_and(t_ast *tree, bool forked)
{
	executor(tree->left, false);
	if (!get_exit_status())
		executor(tree->right, false);
	if (forked)
		exit(get_exit_status());
}

void	exec_subsh(t_ast *tree, bool forked)
{
	pid_t	pid;
	int		exit_status;

	pid = ft_fork_wrapper(NULL, 0);
	if (!pid)
	{
		if (exec_redc(tree->redc, SET))
			exit(get_exit_status());
		executor(tree->left, true);
	}
	waitpid(pid, &exit_status, 0);
	set_exit_status(WEXITSTATUS(exit_status));
	if (forked)
		exit(get_exit_status());
}
