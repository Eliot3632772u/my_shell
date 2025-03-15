/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:50:36 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/13 12:53:06 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	prepare_execution(char ***argv, t_ast_exec *exe)
{
	*argv = expand_args(exe->argv_tok);
	if (!*argv || !(*argv)[0])
	{
		free_list(*argv);
		return (false);
	}
	if (!sanitize_argv(argv, exe))
		return (false);
	log_last_command((*argv)[split_len(*argv) - 1]);
	return (true);
}

bool	handle_builtins(char **argv, bool forked)
{
	if (check_builtins(split_len(argv) - 1, argv[0], argv + 1))
	{
		exit_if_forked(forked);
		free_list(argv);
		return (true);
	}
	return (false);
}

int	handle_builtin_redir(t_ast_redir *tree, int fd_to_dup)
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

int	setup_builtin_redirection(t_ast_redir *tree, int fd_to_dup,
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

void	executor(t_ast_cmd *tree, bool forked)
{
	if (!tree)
	{
		if (forked)
			exit(get_exit_status());
		return ;
	}
	if (tree->type == P_AND)
		exec_and((t_ast_binary *)tree, forked);
	else if (tree->type == P_OR)
		exec_or((t_ast_binary *)tree, forked);
	else if (tree->type == P_PIPE)
		exec_pipe((t_ast_binary *)tree, forked);
	else if (tree->type == P_EXEC)
		exec_exe((t_ast_exec *)tree, forked);
	else if (tree->type == P_REDIR)
		exec_redir((t_ast_redir *)tree, forked);
	else if (tree->type == P_SUBSH)
		exec_subsh((t_ast_subsh *)tree, forked);
}
