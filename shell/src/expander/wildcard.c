/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:26:43 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/07 06:51:29 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	match_star(char *str, char *expr)
{
	while (expr[1] == '*')
		expr++;
	if (wild_match(str, expr + 1))
		return (true);
	return (false);
}

bool	wild_match(char *str, char *expr)
{
	while (*str && *expr)
	{
		if (*expr == '*' && expr[1] == '\0')
			return (true);
		if (*expr == '*')
		{
			if (match_star(str, expr))
				return (true);
			str++;
		}
		else
		{
			if (*str != *expr)
				return (false);
			str++;
			expr++;
		}
	}
	return (*str == '\0' && *expr == '\0');
}

static void	process_directory(DIR *dir, t_str *expr, \
	t_str **lst, bool is_hidden)
{
	struct dirent	*entry;
	bool			matched;

	matched = false;
	entry = readdir(dir);
	while (entry)
	{
		if ((is_hidden || entry->d_name[0] != '.') && \
			wild_match(entry->d_name, expr->str))
		{
			ft_stradd_back(lst, new_str(entry->d_name, false));
			matched = true;
		}
		entry = readdir(dir);
	}
	if (!matched && !(*lst == NULL && expr->str[0] == '*' && \
		expr->str[1] == '\0'))
		ft_stradd_back(lst, new_str(expr->str, false));
}

void	wild_card(t_str **lst, t_str *expr)
{
	DIR		*dir;
	bool	is_hidden;

	is_hidden = (expr->str[0] == '.');
	dir = opendir(".");
	if (!dir)
	{
		ft_stradd_back(lst, new_str(expr->str, false));
		return ;
	}
	if (*lst == NULL && expr->str[0] == '*' && expr->str[1] == '\0')
	{
		process_directory(dir, expr, lst, is_hidden);
		if (!*lst)
		{
			closedir(dir);
			return ;
		}
	}
	else
		process_directory(dir, expr, lst, is_hidden);
	sort_str_list(lst);
	closedir(dir);
}

t_str	*expand_wild_cards(t_str *argv_lst)
{
	t_str	*new_argv;
	t_str	*current;
	t_str	*next;

	new_argv = NULL;
	current = argv_lst;
	while (current)
	{
		next = current->next;
		if (current->wild_card)
			wild_card(&new_argv, current);
		else
			ft_stradd_back(&new_argv, new_str(current->str, false));
		free_strnode(current);
		current = next;
	}
	return (new_argv);
}
