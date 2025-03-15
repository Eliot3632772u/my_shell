/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_io.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:47:45 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/15 17:01:31 by irabhi           ###   ########.fr       */
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

bool	is_delimiter(char *line, char *delim)
{
	return (ft_strlen(line) == ft_strlen(delim)
		&& !ft_strncmp(delim, line, ft_strlen(delim)));
}

bool	handle_heredoc_line(char *line, char *delim, \
	int fd)
{
	if (is_delimiter(line, delim))
		return (true);
	ft_putendl_fd(line, fd);
	free(line);
	return (false);
}
