/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:51:55 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/14 13:03:07 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	reset_stdin(void)
{
	int	fd;

	fd = open("/dev/tty", O_RDONLY);
	if (fd == -1)
		return ;
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	handle_heredoc_signal(void)
{
	if (g_last_signal != 420)
		return ;
	reset_stdin();
}

int	process_heredoc_input(int fd, char *delim, t_heredoc_opts opts)
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

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	set_exit_status(130);
	g_last_signal = 420;
	rl_free_line_state();
	rl_cleanup_after_signal();
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	close(STDIN_FILENO);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
