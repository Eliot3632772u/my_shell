/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:56:56 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/13 12:57:57 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_dup2_wrapper(int oldfd, int newfd)
{
	int	ret;

	ret = dup2(oldfd, newfd);
	if (ret < 0)
	{
		print_err("dup2", 0);
		set_exit_status(1);
	}
	return (ret);
}

pid_t	ft_fork_wrapper(int *fds_to_close, int num_fds)
{
	pid_t	pid;
	int		i;

	pid = fork();
	if (pid < 0)
	{
		print_err("fork", 0);
		exit(1);
	}
	else if (pid == 0 && fds_to_close)
	{
		i = 0;
		while (i < num_fds)
		{
			if (fds_to_close[i] >= 0)
				close(fds_to_close[i]);
			i++;
		}
	}
	return (pid);
}

pid_t	ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		print_err("fork", 0);
		exit(1);
	}
	return (pid);
}

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
	}
}
