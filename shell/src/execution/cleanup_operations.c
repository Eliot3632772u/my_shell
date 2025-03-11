/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers_9.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 08:35:32 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/11 08:38:30 by yrafai           ###   ########.fr       */
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

void	handle_child_redir(t_ast_redir *tree, int fd_to_dup)
{
	if (fd_to_dup != tree->fd && ft_dup2_wrapper(fd_to_dup, tree->fd) < 0)
	{
		close(fd_to_dup);
		exit(get_exit_status());
	}
	if (fd_to_dup != tree->fd)
		close(fd_to_dup);
	executor(tree->cmd, true);
	exit(get_exit_status());
}

void	handle_nonforked_redir(t_ast_redir *tree)
{
	int		fd_to_dup;
	char	*file_name;
	pid_t	pid;
	int		status;

	file_name = check_file_tok(tree->file_tok);
	if (!file_name)
		return ;
	fd_to_dup = open_redir_file(tree, file_name, false);
	if (fd_to_dup < 0)
		return ;
	pid = ft_fork_wrapper(NULL, 0);
	if (pid == 0)
		handle_child_redir(tree, fd_to_dup);
	close(fd_to_dup);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		set_exit_status(WEXITSTATUS(status));
}

void	handle_forked_redir(t_ast_redir *tree)
{
	int		fd_to_dup;
	char	*file_name;

	file_name = check_file_tok(tree->file_tok);
	if (!file_name)
		exit(get_exit_status());
	fd_to_dup = open_redir_file(tree, file_name, true);
	if (fd_to_dup < 0)
		exit(get_exit_status());
	handle_child_redir(tree, fd_to_dup);
}
