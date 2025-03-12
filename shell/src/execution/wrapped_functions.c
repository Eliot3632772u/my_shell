/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapped_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 08:04:58 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/12 08:06:23 by yrafai           ###   ########.fr       */
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
