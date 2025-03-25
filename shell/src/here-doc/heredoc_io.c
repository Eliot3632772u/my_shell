/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_io.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:47:45 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/25 02:56:53 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*strip_leading_tabs(char *line)
{
	return (ft_strdup(line + count_leading_tabs(line)));
}

bool	is_delimiter(char *line, char *delim, t_token_type type)
{
	char	*stripped_line;
	bool	result;

	if (line == NULL || delim == NULL)
		return (false);
	if (type == HEREDOC_TAB)
		stripped_line = strip_leading_tabs(line);
	else
		stripped_line = ft_strdup(line);
	if (!stripped_line)
		return (false);
	result = (ft_strlen(stripped_line) == ft_strlen(delim)
			&& !ft_strncmp(delim, stripped_line, ft_strlen(delim)));
	free(stripped_line);
	return (result);
}

static char	*process_heredoc_line(char *line, t_token_type type)
{
	if (type == HEREDOC_TAB)
		return (strip_leading_tabs(line));
	return (ft_strdup(line));
}

static char	*expand_heredoc_line(char *line, t_token_type type)
{
	char	*expanded;

	expanded = expand_env(line, (type == HEREDOC), false);
	if (!expanded)
		expanded = ft_strdup("");
	return (expanded);
}

bool	handle_heredoc_line(t_heredoc_data *data)
{
	char	*processed_line;

	if (is_delimiter(data->line, data->delim, data->type))
	{
		free(data->line);
		return (true);
	}
	processed_line = process_heredoc_line(data->line, data->type);
	if (processed_line)
	{
		if (data->expand)
		{
			processed_line = expand_heredoc_line(processed_line, data->type);
		}
		ft_putendl_fd(processed_line, data->fd);
		free(processed_line);
	}
	free(data->line);
	return (false);
}
