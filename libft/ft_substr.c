/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-10-22 09:29:53 by irabhi            #+#    #+#             */
/*   Updated: 2024-10-22 09:29:53 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	slen;
	size_t	actual_len;

	if (!s)
		return (0);
	slen = ft_strlen(s);
	if (start >= slen)
		return (ft_strdup(""));
	if (slen - start < len)
		actual_len = slen - start;
	else
		actual_len = len;
	sub = (char *)malloc(sizeof(char) * actual_len + 1);
	if (!sub)
		return (0);
	ft_strlcpy(sub, s + start, actual_len + 1);
	return (sub);
}
