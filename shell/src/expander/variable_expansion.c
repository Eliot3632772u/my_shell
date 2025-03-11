/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 07:07:28 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/07 22:32:13 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
