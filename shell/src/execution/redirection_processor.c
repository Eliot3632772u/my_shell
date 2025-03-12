/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_processor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 08:11:15 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/12 08:13:24 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	setup_builtin_redirection(t_ast_redir *tree, int fd_to_dup,
	int *old_fd)
{
	*old_fd = dup(tree->fd);
	if (*old_fd < 0 || ft_dup2_wrapper(fd_to_dup, tree->fd) < 0)
	{
		if (*old_fd >= 0)
			close(*old_fd);
		close(fd_to_dup);
		return (0);
	}
	if (fd_to_dup != tree->fd)
		close(fd_to_dup);
	return (1);
}

static int	handle_builtin_redir(t_ast_redir *tree, int fd_to_dup)
{
	int		old_fd;
	char	*cmd;
	int		is_builtin;

	if (!tree->cmd || tree->cmd->type != P_EXEC || \
		!((t_ast_exec *)tree->cmd)->argv_tok)
		return (0);
	cmd = ((t_ast_exec *)tree->cmd)->argv_tok->value;
	if (!cmd)
		return (0);
	is_builtin = !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset");
	if (!is_builtin)
		return (0);
	if (!setup_builtin_redirection(tree, fd_to_dup, &old_fd))
		return (0);
	executor(tree->cmd, 0);
	dup2(old_fd, tree->fd);
	close(old_fd);
	return (1);
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
	executor(tree->cmd, true);
	exit(get_exit_status());
}
