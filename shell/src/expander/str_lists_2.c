/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_lists_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:07:44 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/04 00:13:48 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	split_len(char **lst)
{
	int	len;

	len = 0;
	while (lst[len])
		len++;
	return (len);
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

void	add_str_lst(char *str, t_str **lst, bool join_to_last, t_token *tok)
{
	bool	to_expand;
	t_str	*node;

	to_expand = (tok->type == WORD && ft_strchr(str, '*'));
	if (tok->type == WORD && !ft_strncmp(str, "$", 2) && tok->nospace_next)
	{
		free(str);
		return ;
	}
	if (join_to_last)
		ft_join_last(lst, str, to_expand);
	else
	{
		node = ft_malloc(sizeof(t_str));
		if (!node)
		{
			free(str);
			return ;
		}
		node->str = str;
		node->wild_card = to_expand;
		node->next = NULL;
		ft_stradd_back(lst, node);
	}
}

char	**consume_argv(t_str *lst)
{
	char	**argv;
	t_str	*tmp;
	int		i;
	int		len;

	len = ft_strlstlen(lst);
	argv = ft_malloc((len + 1) * sizeof(char *));
	i = 0;
	while (i < len)
	{
		argv[i++] = lst->str;
		tmp = lst;
		lst = lst->next;
		free(tmp);
	}
	argv[i] = NULL;
	return (argv);
}
