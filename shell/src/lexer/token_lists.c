/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lists.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:28:53 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/04 00:28:54 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*ft_toklast(t_token *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_tokadd_back(t_token **lst, t_token *new_tok)
{
	if (!lst || !new_tok)
		return ;
	if (!(*lst))
	{
		*lst = new_tok;
		return ;
	}
	ft_toklast(*lst)->next = new_tok;
}

void	ft_tokiter(t_token *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst);
		lst = lst->next;
	}
}

void	add_to_sublist(t_token **list, t_token *new_tok)
{
	t_token	*ptr;

	if (!*list)
	{
		*list = new_tok;
		return ;
	}
	ptr = *list;
	while (ptr->nospace_next)
		ptr = ptr->nospace_next;
	ptr->nospace_next = new_tok;
}

bool	add_token(t_token **tokens, t_token *token, bool is_space)
{
	t_token	*last_tok;

	if (!token)
		return (false);
	last_tok = ft_toklast(*tokens);
	if (!last_tok || is_space || last_tok->type >= OUTPUT
		|| token->type >= OUTPUT)
	{
		return (ft_tokadd_back(tokens, token), true);
	}
	add_to_sublist(&last_tok->nospace_next, token);
	return (true);
}
