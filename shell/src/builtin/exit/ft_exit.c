#include "../../../includes/minishell.h"

static int	handle_int(char *str_number, int len, int is_negative)
{
	const char	*max_pos = "9223372036854775807";
	const char	*max_neg = "9223372036854775808";
	const char	*limit;

	if (len > 19)
		return (0);
	if (is_negative)
		limit = max_neg;
	else
		limit = max_pos;
	if (len == 19 && ft_strncmp(str_number, limit, 19) > 0)
		return (0);
	return (1);
}

static int	check_arg(char *arg)
{
	char	*trimmed;
	char	*num_start;
	int		is_negative;
	int		i;

	if (!arg || !(trimmed = ft_strtrim(arg, " \t\n\v\f\r")))
		return (0);
	num_start = trimmed;
	is_negative = 0;
	if (*num_start == '+' || *num_start == '-')
	{
		if (*num_start == '-')
			is_negative = 1;
		num_start++;
	}
	if (!*num_start)
		return (free(trimmed), 0);
	while (*num_start == '0' && *(num_start + 1))
		num_start++;
	i = -1;
	while (num_start[++i])  // Replace ft_str_is_digit with this loop
		if (!ft_isdigit(num_start[i]))
			return (free(trimmed), 0);
	if (!handle_int(num_start, i, is_negative))
		return (free(trimmed), 0);
	free(trimmed);
	return (1);
}

int	ft_exit(int argc, char *arg)
{
	if (arg && !check_arg(arg))
	{
		ft_putstr_fd("Minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		exit(2);
	}
	if (argc > 1)
	{
		ft_putendl_fd("Minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (arg)
		set_exit_status(ft_atoi(arg));
	exit(get_exit_status() % 256);
}