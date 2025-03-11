/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 01:03:24 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/04 01:05:06 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	valid_env_char(char c)
{
	return (ft_isalpha(c) || ft_isdigit(c) || ft_strchr("?_", c));
}

static bool	is_quoted_dollar_var(char *str)
{
	char	*content_start;
	char	*quote_end;
	char	*temp;

	if (str[0] != '$' || !str[1])
		return (false);
	if (str[1] != '"' && str[1] != '\'')
		return (false);
	if (str[1] == '\'')
		return (false);
	content_start = str + 2;
	quote_end = ft_strchr(content_start, str[1]);
	if (!quote_end)
		return (false);
	temp = ft_strchr(content_start, '$');
	return (temp && temp < quote_end && valid_env_char(temp[1]));
}

bool	check_expanding(char *str, t_token_type type)
{
	char	*wild_pos;
	char	*dollar_pos;

	wild_pos = NULL;
	if (type != DQSTR)
		wild_pos = ft_strchr(str, '*');
	dollar_pos = ft_strchr(str, '$');
	if (!wild_pos && !dollar_pos)
		return (false);
	if (is_quoted_dollar_var(str))
		return (true);
	if (dollar_pos && (valid_env_char(dollar_pos[1]) || dollar_pos[1] == '$'))
		return (true);
	if (wild_pos)
		return (true);
	return (false);
}

void	free_tok_lst(t_token *tok)
{
	t_token	*tmp;

	if (!tok)
		return ;
	while (tok)
	{
		free(tok->value);
		free_tok_lst(tok->nospace_next);
		tmp = tok;
		tok = tok->next;
		free(tmp);
	}
}
