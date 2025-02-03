/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 09:59:45 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 09:59:45 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	skip_token(char *var_name, char **input, t_token_type type)
{
	if (type == WORD)
	{
		if (**input == '*')
		{
			(*input)++;
			free(var_name);
			return (1);
		}
	}
	return (0);
}

void	last_check_concat(t_token *token)
{
	if (token == NULL)
		return ;
	while (token && token->next)
		token = token->next;
	token->concate = 0;
}

void	lexer(t_shell *shell, t_token **token, char *input)
{
	char	*start;
	char	*value;

	while (*input)
	{
		while (*input && ft_isspace(*input))
			input++;
		if (*input && is_special(*input))
			handle_special(shell, token, &input);
		else if (*input && !is_special(*input))
		{
			start = input;
			while (*input && !is_delem(&input) && *input != '*' && \
			*input != '$' && *input != '"' && *input != '\'')
				input++;
			value = ft_substr(start, 0, input - start);
			if (value == NULL)
				shell->error = ERR_MEMORY;
			tokenize(shell, token, value, WORD);
			check_concate(*token, &input);
		}
		if (shell->error)
			return ;
	}
	last_check_concat(*token);
}
