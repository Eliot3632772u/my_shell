/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lists.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:04:28 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/03 23:04:29 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*ft_lstnew2(char *key, char *value, int equal_sign, t_env *prev)
{
	t_env	*node;

	node = (t_env *)ft_malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->prev = prev;
	node->next = NULL;
	node->equal_sign = equal_sign;
	return (node);
}

void	ft_lstadd_back2(t_env **lst, t_env *new)
{
	t_env	*ptr;

	if (!lst || !new)
		return ;
	ptr = *lst;
	if (ptr)
		ft_lstlast2(ptr)->next = new;
	else
		*lst = new;
}

void	ft_lstdelone2(t_env *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->key);
	del(lst->value);
	lst->key = NULL;
	lst->value = NULL;
	lst->prev = NULL;
	lst->next = NULL;
	free(lst);
}

int	ft_lstsize2(t_env *lst)
{
	t_env	*ptr;
	size_t	count;

	ptr = lst;
	if (!ptr)
		return (0);
	count = 1;
	while (ptr->next)
	{
		ptr = ptr->next;
		count++;
	}
	return (count);
}

t_env	*ft_lstlast2(t_env *lst)
{
	t_env	*ptr;

	if (!lst)
		return (NULL);
	ptr = lst;
	while (ptr->next)
		ptr = ptr->next;
	return (ptr);
}
