#include "../../includes/minishell.h"

void	lex_err(char *str, t_shell *shell)
{
	free(str);
	free(shell->input);
	ft_putendl_fd(ERR_QUOTE, STD_ERR);
	shell->error = 2;
}

int check_special_case(t_shell *shell, char ***input, char **str)
{
	char *tmp;

	tmp = ++(**input);
	if (*tmp == '?')
	{
		shell->is_exit_status = 1;
		add_char(shell, str, ft_strdup("$?"));
		return (1);
	}
	else if (!ft_isalnum(*tmp) || *tmp != '_')
	{
		add_char(shell, str, ft_strdup("$"));
		return (1);
	}
	return (0);
}
