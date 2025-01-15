/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-10-22 12:21:20 by irabhi            #+#    #+#             */
/*   Updated: 2024-10-22 12:21:20 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_mem(char **arr, size_t size)
{
	size_t		i;

	i = 0;
	while (i < size)
		free(arr[i++]);
	free(arr);
}

static int	ft_countword(char const *s, char c)
{
	size_t		count;

	if (!*s)
		return (0);
	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s != c && *s)
			s++;
	}
	return (count);
}

static int	wordl(char const *s, char **result, char c, size_t words)
{
	size_t		i;
	size_t		j;
	size_t		count;

	j = 0;
	count = 0;
	while (count < words)
	{
		i = 0;
		while (s[j] == c && s[j])
			j++;
		while (s[j] != c && s[j])
		{
			j++;
			i++;
		}
		result[count] = (char *)malloc((i + 1) * sizeof(char));
		if (!result[count])
		{
			free_mem(result, count);
			return (1);
		}
		result[count++][i] = '\0';
	}
	return (0);
}

static void	fill(char const *s, char **lst, char c, size_t words)
{
	size_t		i;
	size_t		count;
	size_t		j;

	count = 0;
	i = 0;
	while (count < words)
	{
		j = 0;
		while (s[i] == c && s[i])
			i++;
		while (s[i] != c && s[i])
		{
			lst[count][j] = s[i];
			i++;
			j++;
		}
		count++;
	}
}

char	**ft_split(char const *s, char c)
{
	char	**lst;
	size_t	words;

	if (!s)
		return (0);
	words = ft_countword(s, c);
	lst = (char **)malloc((words + 1) * sizeof(char *));
	if (!lst)
		return (0);
	lst[words] = 0;
	if (wordl(s, lst, c, words))
		return (0);
	fill(s, lst, c, words);
	return (lst);
}
