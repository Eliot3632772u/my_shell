/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 09:25:06 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 09:25:06 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_heredoc_delem(t_token *token, t_shell *shell)
{
	char	*arg;

	arg = NULL;
	while (token)
	{
		join_word(&arg, token->value, shell);
		if (shell->error)
		{
			if (arg)
				free(arg);
			return (NULL);
		}
		token = token->next;
	}
	return (arg);
}

int	count_tokens(t_token *tokens)
{
	int		i;

	i = 0;
	while (tokens)
	{
		tokens = tokens->next;
		i++;
	}
	return (i);
}

int	count_arr(char **args)
{
	int		i;

	i = 0;
	while (args && args[i])
		i++;
	return (i);
}

char	*ambiguous_redir(t_shell *shell, char *arg, char **args)
{
	shell->error = AMBIGUOUS_REDIC;
	ft_putstr_fd("minishell: ", STD_ERR);
	ft_putstr_fd(arg, STD_ERR);
	ft_putendl_fd(" ambiguous redirect", STD_ERR);
	free(arg);
	free_arr(args);
	return (NULL);
}

char	*expand_redec(t_token *token, t_shell *shell)
{
	char	**arg;
	char	*tmp;

	arg = NULL;
	if (token->type == HEREDOC)
	{
		tmp = get_heredoc_delem(token, shell);
		if (shell->error)
			return (NULL);
		return (tmp);
	}
	tmp = join_tokens(token, shell, count_tokens(token));
	insert_arg(&arg, tmp, shell);
	if (shell->error)
	{
		free(arg);
		return (NULL);
	}
	if (count_arr(arg) > 1 && shell->is_wild)
		ambiguous_redir(shell, tmp, arg);
	tmp = *arg;
	free(arg);
	return (tmp);
}
