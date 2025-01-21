#include "../../includes/minishell.h"

void	handle_error(t_shell *shell, t_token *tokens, char *str, char *error)
{
	cleanup_shell(shell);
	if (str)
		free(str);
	if (tokens)
		free_tokens(tokens);
	ft_putstr_fd(error, 2);
	exit(EXIT_FAILURE);
}
