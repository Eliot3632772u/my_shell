/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 01:05:35 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/11 02:31:41 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_quoted_word(char *str)
{
	char	quote;
	char	*pos;

	if (str[0] != '$' || !str[1])
		return (0);
	if (str[1] != '"' && str[1] != '\'')
		return (0);
	quote = str[1];
	pos = ft_strchr(str + 2, quote);
	if (!pos)
		return (1);
	return (pos - str + 1);
}

static int	handle_quoted_length(char *str, char quote)
{
	char	*pos;

	pos = ft_strchr(str + 1, quote);
	if (pos)
		return (pos - str + 1);
	return (1);
}

static int	handle_special_types(t_token_type type, char *str)
{
	int		quoted_len;

	if (type == WORD)
	{
		if (str[0] == '$' && (str[1] == '?' || str[1] == '$'))
			return (2);
		quoted_len = handle_quoted_word(str);
		if (quoted_len > 0)
			return (quoted_len);
		return (count_with_func(str, ft_isspecial));
	}
	if (type == WHITE_SPACE)
		return (count_with_func(str, ft_isspace));
	if (type == DQSTR)
		return (handle_quoted_length(str, '"'));
	if (type == STR)
		return (handle_quoted_length(str, '\''));
	return (-1);
}

int	token_len(t_token_type type, char *str)
{
	const char	*lengths = "\0\0\0\1\2\1\2\3\1\1\1\2\2\1\1";
	int			special_len;

	if ((int)type < 0)
		return (1);
	if (lengths[type] > 0)
		return (lengths[type]);
	special_len = handle_special_types(type, str);
	if (special_len > 0)
		return (special_len);
	return (1);
}
