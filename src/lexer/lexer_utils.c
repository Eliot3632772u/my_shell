# include "../includes/minishell.h"

int	is_spaces(char c)
{
	if (c >= 9 && c <= 13 || c == 32)
		return (1);
	return (0);
}

int	is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

int	is_special(char c)
{
	int		i;
	char 	*special ;

	special = "<>|$&()";
	i = 0;
	while (special[i])
	{
		if (special[i] == c)
			return (1);
		i++;
	}
	return (0);
}

