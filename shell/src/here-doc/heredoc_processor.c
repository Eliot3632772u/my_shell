/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 11:00:09 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/11 03:19:14 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_heredoc_input(int fd, char *delim, t_heredoc_opts opts)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || g_last_signal == 420)
			return (g_last_signal == 420);
		if (handle_heredoc_line(line, delim, fd, opts))
		{
			free(line);
			return (0);
		}
	}
}

char	*handle_heredoc(char *delim, t_heredoc_opts opts)
{
	char	*tmp_file;
	int		fd;
	int		interrupted;

	if (!init_heredoc(delim, &tmp_file, &fd))
		return (NULL);
	signal(SIGINT, heredoc_sigint_handler);
	interrupted = process_heredoc_input(fd, delim, opts);
	close(fd);
	if (interrupted)
	{
		cleanup_heredoc_file(tmp_file);
		return (NULL);
	}
	return (tmp_file);
}

char	*ft_mktmp(void)
{
	t_strbuilder	*sb;
	char			*name;
	int				fd;
	pid_t			pid;

	pid = getpid();
	sb = stringbuilder();
	sb_append(sb, "/tmp/.Minishell_HEREDOC_");
	sb_append_int(sb, pid);
	name = ft_strdup(sb->str);
	sb_free(sb);
	fd = open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (fd != -1)
	{
		unlink(name);
		close(fd);
		return (name);
	}
	free(name);
	return (NULL);
}

int	process_heredoc_tree(t_ast_cmd *tree)
{
	int	type;

	if (!tree)
		return (1);
	type = tree->type;
	if (type == P_AND || type == P_OR || type == P_PIPE)
	{
		if (!process_heredoc_tree(((t_ast_binary *)tree)->left))
			return (0);
		return (process_heredoc_tree(((t_ast_binary *)tree)->right));
	}
	else if (tree->type == P_SUBSH)
		return (process_heredoc_tree(((t_ast_subsh *)tree)->cmd));
	else if (tree->type == P_REDIR)
	{
		if (((t_ast_redir *)tree)->direction == HEREDOC
			|| ((t_ast_redir *)tree)->direction == HEREDOC_TAB)
			if (!patch_token((t_ast_redir *)tree))
				return (0);
		return (process_heredoc_tree(((t_ast_redir *)tree)->cmd));
	}
	return (1);
}

int	ft_heredoc(t_ast_cmd *tree)
{
	if (!process_heredoc_tree(tree))
		return (0);
	return (g_last_signal != 420);
}
