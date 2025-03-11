/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:54:48 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/04 01:06:00 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*new_token(t_token_type type, char *value, int len)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->len = len;
	token->to_expand = false;
	if (type == DQSTR || type == WORD)
		token->to_expand = check_expanding(value, type);
	token->nospace_next = NULL;
	token->next = NULL;
	return (token);
}

static t_token_type	check_heredoc(char *str)
{
	if (str[1] == '<')
	{
		if (str[2] == '-')
			return (HEREDOC_TAB);
		return (HEREDOC);
	}
	return (INPUT);
}

t_token_type	token_type(char *str)
{
	if (ft_isspace(str[0]))
		return (WHITE_SPACE);
	if (str[0] == '|')
		return (PIPE + (str[1] == '|'));
	if (str[0] == '\'')
		return (STR);
	if (str[0] == '"')
		return (DQSTR);
	if (str[0] == '>')
		return (OUTPUT + (str[1] == '>'));
	if (str[0] == '<')
		return (check_heredoc(str));
	if (str[0] == '(')
		return (LPREN);
	if (str[0] == ')')
		return (RPREN);
	if (str[0] == '&')
	{
		if (str[1] == '&')
			return (AND);
		return (-1);
	}
	return (WORD);
}

int	count_with_func(char *str, bool (*test_func)(char))
{
	int	len;

	len = 0;
	while (test_func(*str++))
		len++;
	return (len);
}
