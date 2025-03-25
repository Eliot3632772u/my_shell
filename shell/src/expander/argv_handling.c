/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:26:41 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/25 01:34:07 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**expand_args(t_token *tok_lst)
{
	t_str	*argv_lst;
	t_str	*args;
	char	**result;

	if (!tok_lst)
		return (NULL);
	argv_lst = process_command(tok_lst);
	args = process_arguments(tok_lst->next);
	result = NULL;
	if (argv_lst)
		result = handle_argv_cases(argv_lst, args);
	else if (args)
		result = consume_argv(args);
	else if (tok_lst->type == DQSTR || tok_lst->type == STR)
		result = handle_empty_str_case();
	if (!result && args)
		free_str_lst(args);
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
