/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_lists_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 17:53:00 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/06 17:54:34 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_str	*sort_merge(t_str *left, t_str *right)
{
	t_str	*result;

	if (!left)
		return (right);
	if (!right)
		return (left);
	if (ft_strcmp(left->str, right->str) <= 0)
	{
		result = left;
		result->next = sort_merge(left->next, right);
	}
	else
	{
		result = right;
		result->next = sort_merge(left, right->next);
	}
	return (result);
}

void	split_list(t_str *source, t_str **left, t_str **right)
{
	t_str	*fast;
	t_str	*slow;

	slow = source;
	fast = source->next;
	while (fast)
	{
		fast = fast->next;
		if (fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*left = source;
	*right = slow->next;
	slow->next = NULL;
}

void	sort_str_list(t_str **lst)
{
	t_str	*head;
	t_str	*left;
	t_str	*right;

	head = *lst;
	if (!head || !head->next)
		return ;
	split_list(head, &left, &right);
	sort_str_list(&left);
	sort_str_list(&right);
	*lst = sort_merge(left, right);
}
