/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_special_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 09:52:45 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 09:52:45 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	token_var(t_shell *shell, t_token **token, char **input)
{
	char			*var_name;
	char			*start;
	t_token_type	type;

	start = (*input)++;
	while (**input && (ft_isalnum(**input) || **input == '_' || **input == '?'))
	{
		if (check_expand_status(input))
			break ;
		(*input)++;
	}
	var_name = ft_substr(start, 0, *input - start);
	if (!var_name)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	type = VARIABL;
	if (ft_strcmp(var_name, "$") == 0)
		type = WORD;
	if (skip_token(var_name, input, type))
		return ;
	tokenize(shell, token, var_name, type);
	check_concate(*token, input);
}

void	token_and(t_shell *shell, t_token **token, char **input)
{
	char	*value;

	if (*(*input + 1) == '&')
	{
		value = ft_strdup("&&");
		if (value == NULL)
		{
			shell->error = ERR_MEMORY;
			return ;
		}
		tokenize(shell, token, value, T_AND);
		(*input) += 2;
		return ;
	}
	value = ft_strdup("&");
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tokenize(shell, token, value, WORD);
	check_concate(*token, input);
	(*input)++;
}

void	token_wild(t_shell *shell, t_token **token, char **input)
{
	char	*start;
	char	*value;

	start = *input;
	while (**input && !is_wild_special(input))
		(*input)++;
	value = ft_substr(start, 0, *input - start);
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tokenize(shell, token, value, WILD);
	check_concate(*token, input);
}

void	token_paran(t_shell *shell, t_token **token, char **input)
{
	char			*value;
	t_token_type	type;

	if (**input == '(')
	{
		value = ft_strdup("(");
		type = O_P;
	}
	else
	{
		value = ft_strdup(")");
		type = C_P;
	}
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tokenize(shell, token, value, type);
	(*input)++;
}

void	token_pipe(t_shell *shell, t_token **token, char **input)
{
	char			*value;
	t_token_type	type;

	if (*(*input + 1) == '|')
	{
		value = ft_strdup("||");
		type = T_OR;
		(*input)++;
	}
	else
	{
		value = ft_strdup("|");
		type = PIPE;
	}
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tokenize(shell, token, value, type);
	(*input)++;
}
