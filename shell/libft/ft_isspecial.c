#include "libft.h"

bool	ft_isspecial(char c)
{
	if (c == '\0' || ft_isspace(c))
		return (false);
	return (!str_contains("|\'\"><()&", c));
}

bool	str_contains(char *str, char c)
{
	if (!str)
		return (false);
	while (*str)
	{
		if (*str == c)
			return (true);
		str++;
	}
	return (false);
}