/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:41:18 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/15 10:42:30 by yrafai           ###   ########.fr       */
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
	if (old_to_expand || to_expand)
		str_last->wild_card = true;
	else
		str_last->wild_card = false;
	free(str);
}

void	ft_stradd_back(t_str **lst, t_str *new)
{
	if (!lst || !new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	ft_strlast(*lst)->next = new;
}

void	free_str_lst(t_str *lst)
{
	t_str	*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		if (tmp->str)
			free(tmp->str);
		free(tmp);
	}
}
