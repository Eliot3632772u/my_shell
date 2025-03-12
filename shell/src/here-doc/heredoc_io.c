/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_io.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 11:07:05 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/12 00:05:00 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	count_leading_tabs(char *line)
{
	size_t	i;

	i = 0;
	if (!line || !*line)
		return (0);
	while (line[i] && line[i] == '\t')
		i++;
	return (i);
}

char	*strip_leading_tabs(char *line)
{
	return (ft_strdup(line + count_leading_tabs(line)));
}

bool	is_delimiter(char *line, char *delim, t_heredoc_opts opts)
{
	char	*processed;
	bool	result;

	if (opts.strip_tabs)
	{
		processed = strip_leading_tabs(line);
		result = (ft_strlen(processed) == ft_strlen(delim)
				&& !ft_strncmp(delim, processed, ft_strlen(delim)));
		free(processed);
		return (result);
	}
	return (ft_strlen(line) == ft_strlen(delim)
		&& !ft_strncmp(delim, line, ft_strlen(delim)));
}

void	process_and_write_line(char *line, int fd, t_heredoc_opts opts)
{
	char	*processed_line;
	char	*expanded_line;

	if (opts.strip_tabs)
		processed_line = strip_leading_tabs(line);
	else
		processed_line = ft_strdup(line);
	expanded_line = expand_env(processed_line, false, !opts.expandable);
	ft_putendl_fd(expanded_line, fd);
	free(expanded_line);
}

bool	handle_heredoc_line(char *line, char *delim, \
	int fd, t_heredoc_opts opts)
{
	if (is_delimiter(line, delim, opts))
		return (true);
	process_and_write_line(line, fd, opts);
	return (false);
}
