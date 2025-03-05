/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_lists_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:13:27 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/04 00:13:55 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	add_new_node(t_str **lst, char *str, bool to_expand)
{
	t_str	*node;

	node = ft_malloc(sizeof(t_str));
	if (!node)
	{
		free(str);
		return ;
	}
	node->str = str;
	node->wild_card = to_expand;
	node->next = NULL;
	*lst = node;
}

void	ft_join_last(t_str **lst, char *str, bool to_expand)
{
	t_str	*str_last;
	char	*joined;
	bool	old_to_expand;

	str_last = ft_strlast(*lst);
	if (!str_last)
	{
		add_new_node(lst, str, to_expand);
		return ;
	}
	joined = ft_strjoin(str_last->str, str);
	old_to_expand = str_last->wild_card;
	free(str_last->str);
	str_last->str = joined;
	str_last->wild_card = old_to_expand || to_expand;
	free(str);
}
