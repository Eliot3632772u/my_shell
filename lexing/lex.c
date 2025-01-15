/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-15 09:16:57 by irabhi            #+#    #+#             */
/*   Updated: 2025-01-15 09:16:57 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		lex(t_token **token, char *line)
{
	int		i;

	i = 0;
	while (*(line + i))
	{
		while (is_spaces(line[i]))
			i++;
		if (is_special(line[i]))
			handel_special(token, line, &i);
		else if (is_quote(line[i]))
			handel_quote(token, line, &i);
	}	
}