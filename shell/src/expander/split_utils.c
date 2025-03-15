/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:28:44 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/15 10:40:23 by yrafai           ###   ########.fr       */
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

void	handle_split_args(t_split_args *args)
{
	char	**splited;
	char	*dup;
	int		i;

	splited = ft_split(args->to_join, args->split_char);
	if (!splited)
		return ;
	i = 0;
	while (splited[i])
	{
		dup = ft_strdup(splited[i]);
		if (!dup)
			break ;
		add_str_lst(dup, args->lst, (i == 0 && args->iter != 0), args->sub_tok);
		i++;
	}
	free_list(splited);
	free(args->to_join);
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
