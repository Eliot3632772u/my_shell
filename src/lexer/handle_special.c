/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_special.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 09:41:34 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 09:41:34 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	out_token(t_shell *shell, t_token **token, char **input)
{
	char			*value;
	t_token_type	type;

	if (*(*input + 1) == '>')
	{
		value = ft_strdup(">>");
		type = APPEND;
		(*input)++;
	}
	else
	{
		value = ft_strdup(">");
		type = REDIRECT_OUT;
	}
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tokenize(shell, token, value, type);
	(*input)++;
}

void	input_token(t_shell *shell, t_token **token, char **input)
{
	char			*value;
	t_token_type	type;

	if (*(*input + 1) == '<')
	{
		value = ft_strdup("<<");
		type = HEREDOC;
		(*input)++;
	}
	else
	{
		value = ft_strdup("<");
		type = REDIRECT_IN;
	}
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tokenize(shell, token, value, type);
	(*input)++;
}

void	token_quote(t_shell *shell, t_token **token, char *value, int expand)
{
	if (value == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	if (expand)
		tokenize(shell, token, value, WORD_VAR);
	else
		tokenize(shell, token, value, WORD);
}

void	handle_quotes(t_shell *shell, t_token **token, char **input)
{
	char	*start;
	char	*value;
	char	quote;
	int		expand;

	quote = **input;
	start = ++(*input);
	expand = 0;
	while (**input && **input != quote)
	{
		if (**input == '$' && quote == '"')
			expand = 1;
		(*input)++;
	}
	if (**input != quote)
	{
		shell->error = ERR_QUOTE;
		ft_putendl_fd(ERR_QUOTE_MSG, STD_ERR);
		return ;
	}
	value = ft_substr(start, 0, *input - start);
	token_quote(shell, token, value, expand);
	(*input)++;
	check_concate(*token, input);
}

void	handle_special(t_shell *shell, t_token **token, char **input)
{
	if (**input == '<')
		input_token(shell, token, input);
	else if (**input == '>')
		out_token(shell, token, input);
	else if (**input == '|')
		token_pipe(shell, token, input);
	else if (**input == ')' || **input == '(')
		token_paran(shell, token, input);
	else if (**input == '$')
		token_var(shell, token, input);
	else if (**input == '&')
		token_and(shell, token, input);
	else if (**input == '*')
		token_wild(shell, token, input);
	else if (**input == '"' || **input == '\'')
		handle_quotes(shell, token, input);
}
