/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 09:56:44 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 09:56:44 by irabhi           ###   ########.fr       */
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

int	is_wild_special(char **input)
{
	if (is_delem(input))
		return (1);
	if (**input == '\'' || **input == '"' || **input == '$')
		return (1);
	return (0);
}

int	is_special(char c)
{
	int		i;
	char	*special;

	special = "<>|$&()\"'*";
	i = 0;
	while (special[i])
	{
		if (special[i] == c)
			return (1);
		i++;
	}
	return (0);
}

void	check_concate(t_token *token, char **input)
{
	while (token && token->next)
		token = token->next;
	if (**input && !is_delem(input))
		token->concate = 1;
}
