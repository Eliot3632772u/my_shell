#include "libft.h"

static size_t	align(size_t size)
{
	return ((size + (16 - 1)) & ~ (16 - 1));
}

void	sb_reallocate(t_strbuilder *sb, size_t len)
{
	char	*new;

	sb->capacity = align(sb->used_len + len + 1);
	new = ft_calloc(1, sb->capacity);
	ft_memcpy(new, sb->str, sb->used_len);
	free(sb->str);
	sb->str = new;
}
