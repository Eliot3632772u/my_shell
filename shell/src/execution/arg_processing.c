/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_processing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 12:46:57 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/15 16:52:40 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	shift_argv(char ***argv)
{
	int	i;

	i = 0;
	free((*argv)[0]);
	while ((*argv)[i + 1] != NULL)
	{
		(*argv)[i] = (*argv)[i + 1];
		i++;
	}
	(*argv)[i] = NULL;
}

bool	sanitize_argv(char ***argv, t_ast *exe)
{
	char	*first_arg;
	t_token	*tok;

	first_arg = (*argv)[0];
	tok = exe->argv_tok;
	if (first_arg && ft_strchr(first_arg, ' ') && tok->type != STR \
		&& tok->type != DQSTR)
	{
		shift_argv(argv);
		if ((*argv)[0] == NULL)
		{
			print_err(tok->value, -1);
			set_exit_status(127);
			free_list(*argv);
			return (false);
		}
	}
	return (true);
}

int	validate_expanded_token(t_token *file_tok, char **expanded)
{
	if (ft_strchr(file_tok->value, '$') && ft_strcmp(file_tok->value, "$") != 0)
	{
		if (expanded[0][0] == '\0' || !ft_strcmp(expanded[0], file_tok->value))
		{
			print_err(file_tok->value, -4);
			free_list(expanded);
			return (0);
		}
	}
	return (1);
}

int	validate_expanded_args(t_token *file_tok, char **expanded)
{
	if (!check_expanded_length(file_tok, expanded))
		return (0);
	return (validate_expanded_token(file_tok, expanded));
}

int	check_expanded_length(t_token *file_tok, char **expanded)
{
	int	len;

	len = split_len(expanded);
	if (len == 0)
	{
		free_list(expanded);
		print_err(file_tok->value, -4);
		set_exit_status(1);
		return (0);
	}
	if (len > 1)
	{
		print_err(file_tok->value, -4);
		free_list(expanded);
		return (0);
	}
	return (1);
}
