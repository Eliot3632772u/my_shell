#include "../../../includes/minishell.h"

int	exit_status(unsigned int action)
{
	static int	exit_status;

	if (action != GET_EXIT)
		exit_status = action;
	return (exit_status);
}

void	set_exit_status(unsigned int status)
{
	exit_status(status);
}

int	get_exit_status(void)
{
	return (exit_status(GET_EXIT));
}
