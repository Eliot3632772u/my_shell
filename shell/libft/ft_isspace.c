#include "libft.h"

bool	ft_isspace(char c)
{
	if (c == '\t' || c == '\n' || c == '\f' || c == '\v' || c == ' '
		|| c == '\r')
		return (1);
	return (0);
}