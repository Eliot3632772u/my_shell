/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helpers_5.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 05:44:50 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/10 06:06:08 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**handle_empty_str_case(void)
{
	char	**result;

	result = ft_malloc(2 * sizeof(char *));
	if (result)
	{
		result[0] = ft_strdup("");
		result[1] = NULL;
	}
	return (result);
}

static char	**handle_wildcard_expansion(t_str *argv_lst, t_str *args)
{
	t_str	*final_lst;
	char	**result;

	final_lst = expand_wild_cards(argv_lst);
	if (!final_lst)
	{
		free_str_lst(args);
		return (NULL);
	}
	ft_strlast(final_lst)->next = args;
	result = consume_argv(final_lst);
	return (result);
}

char	**handle_argv_cases(t_str *argv_lst, t_str *args)
{
	char	**result;

	result = NULL;
	if (argv_lst->wild_card)
		result = handle_wildcard_expansion(argv_lst, args);
	else
	{
		ft_strlast(argv_lst)->next = args;
		result = consume_argv(argv_lst);
	}
	return (result);
}
