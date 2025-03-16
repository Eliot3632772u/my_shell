/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_io.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:47:45 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 07:20:27 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t count_leading_tabs(char *line)
{
    size_t i;

    i = 0;
    if (!line || !*line)
        return (0);
    while (line[i] && line[i] == '\t')
        i++;
    return (i);
}

char *strip_leading_tabs(char *line)
{
    return (ft_strdup(line + count_leading_tabs(line)));
}

bool is_delimiter(char *line, char *delim, t_token_type type)
{
    char *stripped_line;
    bool result;

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

bool handle_heredoc_line(char *line, char *delim, t_token_type type, int fd)
{
    char *processed_line;

    if (is_delimiter(line, delim, type))
    {
        free(line);
        return (true);
    }
    if (type == HEREDOC_TAB)
        processed_line = strip_leading_tabs(line);
    else
        processed_line = ft_strdup(line);
    
    if (processed_line)
    {
        ft_putendl_fd(processed_line, fd);
        free(processed_line);
    }
    free(line);
    return (false);
}
