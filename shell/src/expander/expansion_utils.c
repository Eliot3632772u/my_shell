/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 10:31:15 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 21:46:20 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**handle_wildcard_expansion(t_str *argv_lst, t_str *args)
{
	t_str	*final_lst;
	char	**result;

	final_lst = expand_wild_cards(argv_lst);
	if (!final_lst)
	{
		free_str_lst(args);
		return (NULL);
	}
	ft_strlast(final_lst)->next = args;
	result = consume_argv(final_lst);
	return (result);
}

unsigned int	get_chunk_len(char *ptr, char *sp)
{
	unsigned int	len;

	len = 0;
	if (!ft_strchr(sp, *ptr))
	{
		while (ptr[len] && !ft_strchr(sp, ptr[len]))
			len++;
	}
	else
	{
		len++;
		if (ptr[1] == '?' || ptr[1] == '*' || ptr[1] == '$')
			return (len + 1);
		if (ptr[1] && ft_isdigit(ptr[1]))
			return (len + 1);
		while (ft_isalnum(ptr[len]) || ptr[len] == '_')
			len++;
	}
	return (len);
}

void	ghost_char(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t')
			str[i] = (char)HIDDEN_SEPARATOR;
		i++;
	}
}

bool	is_standalone_dollar_star(const char *str)
{
	return (str[0] == '$' && str[1] == '*' && str[2] == '\0');
}

char	*handle_exit_status_2(char *chunk)
{
	free(chunk);
	return (ft_itoa(get_exit_status()));
}
