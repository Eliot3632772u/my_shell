/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:02:32 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/25 02:57:03 by yrafai           ###   ########.fr       */
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

static size_t	count_leading_dollars(char *str)
{
	size_t	count;

	count = 0;
	while (str && *str == '$')
	{
		count++;
		str++;
	}
	return (count);
}

static bool	has_quotes(char *str)
{
	size_t	len;

	if (!str)
		return (false);
	len = ft_strlen(str);
	if (len < 2)
		return (false);
	return ((str[0] == '"' && str[len - 1] == '"')
		|| (str[0] == '\'' && str[len - 1] == '\''));
}

static char	*prepare_quoted_part(char *quoted_part, size_t *dollar_count)
{
	char	*stripped_quoted;

	if (has_quotes(quoted_part))
	{
		stripped_quoted = strip_quotes(quoted_part);
		if (*dollar_count % 2 != 0)
			(*dollar_count)--;
	}
	else
		stripped_quoted = ft_strdup(quoted_part);
	return (stripped_quoted);
}

char	*process_delimiter_with_dollars(char *str)
{
	size_t	dollar_count;
	char	*quoted_part;
	char	*stripped_quoted;
	char	*result;
	char	*temp;

	if (!str)
		return (NULL);
	dollar_count = count_leading_dollars(str);
	quoted_part = str + dollar_count;
	stripped_quoted = prepare_quoted_part(quoted_part, &dollar_count);
	if (!stripped_quoted)
		return (NULL);
	temp = malloc(dollar_count + 1);
	if (!temp)
	{
		free(stripped_quoted);
		return (NULL);
	}
	ft_memset(temp, '$', dollar_count);
	temp[dollar_count] = '\0';
	result = ft_strjoin(temp, stripped_quoted);
	free(temp);
	free(stripped_quoted);
	return (result);
}
