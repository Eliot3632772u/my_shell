/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_core.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:42:31 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 13:55:18 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	process_heredoc_tree(t_ast *tree)
{
	t_redirect	*redc;

	if (tree == NULL)
		return (0);
	process_heredoc_tree(tree->left);
	process_heredoc_tree(tree->right);
	redc = tree->redc;
	while (redc)
	{
		if (redc->type == HEREDOC || redc->type == HEREDOC_TAB)
		{
			heredoc_limit(SET);
			if (!patch_token(redc))
				return (0);
		}
		redc = redc->next;
	}
	return (1);
}

int	process_heredoc_input(int fd, char *delim, t_token_type type)
{
	char	*line;
	bool	done;

	done = false;
	while (!done && g_last_signal != 420)
	{
		line = readline("> ");
		if (!line)
			break ;
		done = handle_heredoc_line(line, delim, type, fd);
	}
	return (g_last_signal == 420);
}

char	*handle_heredoc(char *delim, t_token_type type)
{
	char	*tmp_file;
	int		fd;
	int		interrupted;

	if (!init_heredoc(delim, &tmp_file, &fd))
		return (NULL);
	signal(SIGINT, heredoc_sigint_handler);
	interrupted = process_heredoc_input(fd, delim, type);
	close(fd);
	if (interrupted)
	{
		cleanup_heredoc_file(tmp_file);
		return (NULL);
	}
	reset_stdin();
	return (tmp_file);
}

int	init_heredoc(char *delim, char **tmp_file, int *fd)
{
	if (!delim)
		return (0);
	*tmp_file = ft_mktmp();
	*fd = open(*tmp_file, O_RDWR | O_CREAT | O_EXCL, 0640);
	if (*fd < 0)
	{
		print_err(*tmp_file, 0);
		set_exit_status(1);
		return (0);
	}
	return (1);
}

int	ft_heredoc(t_ast *tree)
{
	bool	result;

	heredoc_limit_check(tree);
	if (heredoc_limit(RESET) > 16)
	{
		write(2, "Minishell: maximum here-document count exceeded\n", 48);
		exit(2);
	}
	save_fd(SET);
	if (!process_heredoc_tree(tree))
	{
		save_fd(RESET);
		return (0);
	}
	result = (g_last_signal != 420);
	save_fd(RESET);
	return (result);
}
