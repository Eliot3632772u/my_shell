/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:02:09 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/25 10:19:51 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static int	ft_islower(int c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	return (0);
}

static int	ft_isupper(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	return (0);
}

static int	is_special_char(char c)
{
	return (!ft_isalnum(c) && c != '_' && c != '.' && c != '/');
}

static int	get_char_priority(char c)
{
	if (ft_isdigit(c))
		return (1);
	if (is_special_char(c))
		return (2);
	if (ft_isupper(c))
		return (3);
	if (ft_islower(c))
		return (4);
	return (5);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;
	int	prio1;
	int	prio2;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
		{
			prio1 = get_char_priority(s1[i]);
			prio2 = get_char_priority(s2[i]);
			if (prio1 != prio2)
				return (prio2 - prio1);
			return (s1[i] - s2[i]);
		}
		i++;
	}
	return (0);
}
