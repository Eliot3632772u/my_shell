/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:57:15 by irabhi            #+#    #+#             */
/*   Updated: 2025/03/16 08:17:02 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_delem(char **input)
{
	if (ft_isspace(**input))
		return (1);
	if (**input == '|' || **input == '<' || \
		**input == '>' || **input == ')' || **input == '(')
		return (1);
	if (**input == '&' && *(*input + 1) == '&')
		return (1);
	return (0);
}

int	check_expand_status(char **input)
{
	if (**input == '?')
	{
		if (*(*input - 1) == '$')
			(*input)++;
		return (1);
	}
	return (0);
}

int	is_wild_special(char **input)
{
	if (is_delem(input))
		return (1);
	if (**input == '\'' || **input == '"' || **input == '$')
		return (1);
	return (0);
}

int	special_char(char c)
{
	int		i;
	char	*special;

	special = "<>|&()\"'";
	i = 0;
	while (special[i])
	{
		if (special[i] == c)
			return (1);
		i++;
	}
	return (0);
}

t_token_type	check_type(char quote)
{
	if (quote == '\'')
		return (STR);
	return (DQSTR);
}
