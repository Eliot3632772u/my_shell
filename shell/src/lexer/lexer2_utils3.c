/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 08:46:11 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 08:46:30 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*extract_quoted_value(char **command, char quote)
{
	char	*start;
	char	*value;

	start = (*command)++;
	while (**command && **command != quote)
		(*command)++;
	if (!(**command))
	{
		unclosed_error(quote);
		return (NULL);
	}
	(*command)++;
	value = ft_substr(start, 0, *command - start);
	return (value);
}

void	handle_quotes(char **command, t_token **tokens, int *concate)
{
	char	quote;
	char	*value;

	quote = **command;
	value = extract_quoted_value(command, quote);
	if (!value)
		return ;
	add_token(tokens, new_token(check_type(quote), value, \
	ft_strlen(value)), !(*concate));
	check_concate(command, concate);
}
