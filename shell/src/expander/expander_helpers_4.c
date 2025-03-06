/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helpers_4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 08:25:34 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/06 19:34:13 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	expand_environment_variable(t_chunk_info *info, size_t len)
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
	if (info->ptr[len] == '\0' && (ft_chr(*(info->chunk), ' ')
			|| ft_chr(*(info->chunk), '\t')))
	{
		sb_append(info->sb, *(info->chunk));
		free(*(info->chunk));
		*(info->chunk) = NULL;
		return (false);
	}
	return (true);
}

bool	handle_chunk(t_chunk_info *info)
{
	size_t		len;

	len = get_chunk_len(info->ptr, "$");
	*(info->chunk) = ft_substr(info->ptr, 0, len);
	*(info->offset) = len;
	if (!info->ignore_env && *(info->ptr) == '$' && len > 1)
	{
		if (!expand_environment_variable(info, len))
			return (false);
	}
	return (true);
}

static t_str	*process_command(t_token *cmd_token)
{
	t_str	*argv_lst;
	t_str	*expanded_cmd;
	t_str	*wild_expanded;

	argv_lst = NULL;
	expand_nosp_arg(cmd_token, &argv_lst, 0);
	if (argv_lst)
	{
		expanded_cmd = NULL;
		ft_stradd_back(&expanded_cmd, argv_lst);
		wild_expanded = expand_wild_cards(expanded_cmd);
		if (wild_expanded)
			return (wild_expanded);
	}
	return (argv_lst);
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

char	**expand_args(t_token *tok_lst)
{
	t_str	*argv_lst;
	t_str	*args;

	if (!tok_lst)
		return (NULL);
	argv_lst = process_command(tok_lst);
	args = process_arguments(tok_lst->next);
	if (argv_lst)
		ft_strlast(argv_lst)->next = args;
	else
		argv_lst = args;
	return (consume_argv(argv_lst));
}
