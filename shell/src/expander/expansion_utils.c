/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 08:25:34 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/12 00:49:40 by yrafai           ###   ########.fr       */
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
	if (!info->in_quote && info->ptr[len] == '\0' && expanded
		&& (ft_strchr(*(info->chunk), ' ') || ft_strchr(*(info->chunk), '\t')))
	{
		sb_append(info->sb, *(info->chunk));
		free(*(info->chunk));
		*(info->chunk) = NULL;
		return (false);
	}
	return (true);
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
		result = handle_argv_cases(argv_lst, args);
	else if (args)
		result = consume_argv(args);
	else if (tok_lst->type == DQSTR || tok_lst->type == STR)
		result = handle_empty_str_case();
	if (!result && args)
		free_str_lst(args);
	return (result);
}
