/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 09:40:19 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 09:40:19 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*strip_var(char **arg, char *value, t_shell *shell)
{
	char	*res;
	char	*tmp;
	char	*start;

	if (check_not_alnum(arg, value, shell))
		return (value + 1);
	start = value;
	while (*value && (ft_isalnum(*value) || *value == '_'))
		value++;
	tmp = ft_substr(start, 0, value - start);
	if (tmp == NULL)
	{
		shell->error = ERR_MEMORY;
		return (NULL);
	}
	res = find_env(tmp, shell);
	free(tmp);
	if (res == NULL)
		return (value);
	join_word(arg, res, shell);
	free(res);
	return (value);
}

void	strip_word_var(char **res, char *start, char *value, t_shell *shell)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_substr(start, 0, value - start);
	if (tmp == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	tmp2 = *res;
	*res = ft_strjoin(*res, tmp);
	if (tmp2)
		free(tmp2);
	free(tmp);
	if (res == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
}

void	join_word_var(char **arg, char *value, t_shell *shell)
{
	char	*res;					
	char	*start;

	res = NULL;
	start = value;
	while (*value)
	{
		if (*value == '$')
		{
			strip_word_var(&res, start, value, shell);
			if (shell->error)
				return ;
			value++;
			value = strip_var(&res, value, shell);
			if (shell->error)
				return ;
			start = value;
		}
		else
			value++;
	}
	join_word(arg, res, shell);
	free(res);
}

void	join_variable(char **arg, char *value, t_shell *shell)
{
	char	*tmp;

	value++;
	if (ft_strcmp(value, "?") == 0)
	{
		tmp = ft_itoa(shell->last_status);
		if (tmp == NULL)
		{
			shell->error = ERR_MEMORY;
			return ;
		}
		join_word(arg, tmp, shell);
		free(tmp);
		return ;
	}
	tmp = find_env(value, shell);
	if (shell->error || tmp == NULL)
		return ;
	join_word(arg, tmp, shell);
	free(tmp);
}

char	**expand(t_token *tokens, t_shell *shell)
{
	char	**args;
	char	*arg;

	args = NULL;
	while (tokens)
	{
		arg = get_args(&tokens, shell);
		insert_arg(&args, arg, shell);
		if (shell->error)
		{
			free_arr(args);
			return (NULL);
		}
		shell->is_wild = 0;
	}
	return (args);
}
