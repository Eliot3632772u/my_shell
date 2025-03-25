/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_builder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:01:05 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/25 10:26:49 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

t_strbuilder	*stringbuilder(void)
{
	t_strbuilder	*sb;

	sb = ft_malloc(sizeof(t_strbuilder));
	if (!sb)
		return (NULL);
	sb->capacity = 16;
	sb->str = ft_calloc(1, sb->capacity);
	if (!sb->str)
		return (free(sb), NULL);
	sb->used_len = 0;
	return (sb);
}

t_strbuilder	*sb_append(t_strbuilder *sb, char *str)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (sb->used_len + len + 1 > sb->capacity)
		sb_reallocate(sb, len);
	ft_memcpy(sb->str + sb->used_len, str, len);
	sb->used_len += len;
	return (sb);
}

t_strbuilder	*sb_append_free(t_strbuilder *sb, char *str)
{
	t_strbuilder	*to_return;

	to_return = sb_append(sb, str);
	free(str);
	return (to_return);
}

t_strbuilder	*sb_append_int(t_strbuilder *sb, int nb)
{
	char	*str;

	str = ft_itoa(nb);
	if (!str)
		return (NULL);
	return (sb_append_free(sb, str));
}

void	sb_free(t_strbuilder *sb)
{
	free(sb->str);
	free(sb);
}
