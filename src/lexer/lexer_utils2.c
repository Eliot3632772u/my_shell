#include "../includes/minishell.h"

int check_special_case(t_shell *shell, char ***input, char **str)
{
	char *tmp;

	tmp = ++(**input);
	if (*tmp == '?')
	{
		tmp = ft_itoa(shell->last_status);
		if (!tmp)
			handel_error(); // todo
		add_char(str, tmp);
		return (1);
	}
	else if (!ft_isalnum(*tmp))
	{
		add_char(str, ft_strdup("$"));
		return (1);
	}
	return (0);
}
