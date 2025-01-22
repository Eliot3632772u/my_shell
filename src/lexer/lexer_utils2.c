#include "../../includes/minishell.h"

void	lex_err(char *str, t_shell *shell)
{
	if (str)
		free(str);
	if (shell->input)
		free(shell->input);
	ft_putendl_fd(ERR_QUOTE, STD_ERR);
	shell->error = 2;
}

void	insert_str(t_shell *shell, t_token **token, char **input, char *start)
{
	char	*str;

	str = ft_substr(start, 0, *input - start);
	if (!str)
		handle_error(shell, *token, NULL, ERR_MEMORY);
	tokenize(shell, token, str, shell->type);
	shell->type = NONE;
}

//int check_special_case(t_shell *shell, char ***input, char **str)
//{
//	char *tmp;

//	tmp = ++(**input);
//	if (*tmp == '?')
//	{
//		shell->is_DOLLAR = 1;
//		add_char(shell, str, ft_strdup("$?"));
//		return (1);
//	}
//	else if (!ft_isalnum(*tmp) || *tmp != '_')
//	{
//		add_char(shell, str, ft_strdup("$"));
//		return (1);
//	}
//	return (0);
//}
