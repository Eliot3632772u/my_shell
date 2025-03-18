/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_processing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:24:58 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/18 20:49:43 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_str	*process_command(t_token *cmd_token)
{
	t_str	*argv_lst;
	t_str	*expanded_cmd;

	argv_lst = NULL;
	expand_nosp_arg(cmd_token, &argv_lst, 0);
	if (!argv_lst)
		return (NULL);
	if (cmd_token->type == DQSTR || cmd_token->type == STR)
		return (argv_lst);
	if (cmd_token->type == WORD && cmd_token->to_expand
		&& (!argv_lst->str || !*argv_lst->str))
	{
		free_str_lst(argv_lst);
		return (NULL);
	}
	expanded_cmd = argv_lst;
	return (expanded_cmd);
}

void	add_to_args_list(t_str **args, t_str *new_list)
{
	t_str	*last;

	if (!new_list)
		return ;
	if (!*args)
		*args = new_list;
	else
	{
		last = ft_strlast(*args);
		last->next = new_list;
	}
}

void	handle_wildcard_argument(t_str *arg_list, t_str **args)
{
	t_str	*expanded;

	expanded = expand_wild_cards(arg_list);
	if (expanded)
		add_to_args_list(args, expanded);
}

void	process_single_argument(t_token *arg_token, t_str **args)
{
	t_str	*arg_list;

	arg_list = NULL;
	expand_nosp_arg(arg_token, &arg_list, 0);
	if (!arg_list)
		return ;
	if (arg_token->type == WORD && arg_list->str[0] == '\0')
	{
		free_str_lst(arg_list);
		return ;
	}
	if (arg_list->wild_card)
		handle_wildcard_argument(arg_list, args);
	else
		add_to_args_list(args, arg_list);
}

t_str	*process_arguments(t_token *args_token)
{
	t_str	*args;

	args = NULL;
	while (args_token)
	{
		process_single_argument(args_token, &args);
		args_token = args_token->next;
	}
	return (args);
}
