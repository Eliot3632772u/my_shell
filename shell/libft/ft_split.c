/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:50:40 by yrafai            #+#    #+#             */
/*   Updated: 2024/11/04 23:04:49 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
		{
			i++;
		}
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
		{
			i++;
		}
	}
	return (count);
}

static int	ft_allocate(char **strings, char const *s, char sep)
{
	char		**strings1;
	char const	*temp;

	strings1 = strings;
	while (*s)
	{
		while (*s && *s == sep)
			++s;
		temp = s;
		while (*temp && *temp != sep)
			++temp;
		if (*temp == sep || temp > s)
		{
			*strings1 = ft_substr(s, 0, temp - s);
			if (!*strings1)
				return (0);
			s = temp;
			++strings1;
		}
	}
	*strings1 = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**strings;
	int		count;

	if (!s)
		return (NULL);
	count = ft_count_words(s, c);
	strings = (char **)malloc((count + 1) * sizeof(char *));
	if (!strings)
		return (NULL);
	if (!ft_allocate(strings, s, c))
	{
		count = 0;
		while (strings[count])
		{
			if (strings[count])
				free(strings[count]);
			count++;
		}
		free(strings);
		return (NULL);
	}
	return (strings);
}
