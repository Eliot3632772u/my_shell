/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 01:05:51 by yrafai            #+#    #+#             */
/*   Updated: 2024/10/29 01:40:08 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	needle_len;

	if (needle[0] == '\0')
		return ((char *)haystack);
	needle_len = ft_strlen(needle);
	i = 0;
	while (haystack[i] && i < len)
	{
		if (haystack[i] == needle[0] && i + needle_len <= len)
			if (ft_strncmp(needle, &haystack[i], needle_len) == 0)
				return ((char *)&haystack[i]);
		i++;
	}
	return (NULL);
}
