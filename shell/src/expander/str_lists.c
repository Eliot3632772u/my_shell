/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_lists.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:07:03 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/06 17:53:54 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_str	*new_str(char *str, bool to_expand)
{
	t_str	*node;

	node = ft_malloc(sizeof(t_str));
	if (!node)
		return (NULL);
	node->str = ft_strdup(str);
	node->wild_card = to_expand;
	node->next = NULL;
	return (node);
}

t_str	*ft_strlast(t_str *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

void	free_strnode(t_str *node)
{
	free(node->str);
	free(node);
}

int	ft_strlstlen(t_str *lst)
{
	int	len;

	len = 0;
	while (lst)
	{
		len++;
		lst = lst->next;
	}
	return (len);
}

void	ft_striter(t_str *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst);
		lst = lst->next;
	}
}
