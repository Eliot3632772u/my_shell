/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helpers_4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 08:25:34 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/08 00:51:12 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	expand_environment_variable(t_chunk_info *info, size_t len)
{
	t_env_var	env;
	char		*expanded;

	env.chunk = *(info->chunk);
	env.ptr = info->ptr;
	env.len = len;
	env.in_quote = info->in_quote;
	env.offset = info->offset;
	expanded = handle_env_var(&env);
	*(info->chunk) = expanded;
	if (!*(info->chunk))
		return (false);
	if (info->ptr[len] == '\0' && (ft_strchr(*(info->chunk), ' ')
			|| ft_strchr(*(info->chunk), '\t')))
	{
		sb_append(info->sb, *(info->chunk));
		free(*(info->chunk));
		*(info->chunk) = NULL;
		return (false);
	}
	return (true);
}

static t_str	*process_command(t_token *cmd_token)
{
	t_str	*argv_lst;
	t_str	*expanded_cmd;

	argv_lst = NULL;
	expand_nosp_arg(cmd_token, &argv_lst, 0);
	if (!argv_lst)
		return (NULL);
	expanded_cmd = argv_lst;
	return (expanded_cmd);
}

static t_str	*process_arguments(t_token *args_token)
{
	t_str	*args;

	args = NULL;
	while (args_token)
	{
		expand_nosp_arg(args_token, &args, 0);
		args_token = args_token->next;
	}
	if (args)
		args = expand_wild_cards(args);
	return (args);
}

static char	**handle_wildcard_expansion(t_str *argv_lst, t_str *args)
{
	t_str	*final_lst;

	final_lst = expand_wild_cards(argv_lst);
	if (!final_lst)
		return (NULL);
	ft_strlast(final_lst)->next = args;
	return (consume_argv(final_lst));
}

char	**expand_args(t_token *tok_lst)
{
	t_str	*argv_lst;
	t_str	*args;
	char	**result;

	if (!tok_lst)
		return (NULL);
	argv_lst = process_command(tok_lst);
	args = process_arguments(tok_lst->next);
	result = NULL;
	if (argv_lst)
	{
		if (argv_lst->wild_card)
			result = handle_wildcard_expansion(argv_lst, args);
		else
		{
			ft_strlast(argv_lst)->next = args;
			result = consume_argv(argv_lst);
		}
	}
	else if (args)
		result = consume_argv(args);
	return (result);
}
