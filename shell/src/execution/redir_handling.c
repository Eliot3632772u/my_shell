/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:53:41 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 08:13:13 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_nonforked_redir(t_ast_redir *tree)
{
	int		fd_to_dup;
	char	*file_name;
	pid_t	pid;
	int		status;

	file_name = check_file_tok(tree->file_tok);
	if (!file_name)
		return ;
	fd_to_dup = open_redir_file(tree, file_name, 0);
	if (fd_to_dup < 0)
		return ;
	if (handle_builtin_redir(tree, fd_to_dup))
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
	fd_to_dup = open_redir_file(tree, file_name, 1);
	if (fd_to_dup < 0)
		exit(get_exit_status());
	handle_child_redir(tree, fd_to_dup);
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
	exit(get_exit_status());
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
