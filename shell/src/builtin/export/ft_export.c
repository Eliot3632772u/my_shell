#include "../../../includes/minishell.h"

int	ft_export(int argc, char **argv, t_env **env)
{
	int		i;
	char	*ptr;
	char	*key;
	int		res;

	if (!env)
		return (1);
	if (!argc)
		return (print_exports(*env));
	i = 0;
	res = 0;
	while (i < argc)
	{
		ptr = argv[i++];
		if (validate_arg(ptr))
		{
			res = 1;
			continue ;
		}
		if (handle_concate_case(ptr))
			continue ;
		key = get_key(ptr);
		set_env_value(key, get_value(ptr), !!ft_strchr(ptr, '='));
	}
	return (res);
}

int	handle_concate_case(char *line)
{
	if (!line || !is_concate(line))
		return (0);
	concate_env(get_key(line), get_value(line));
	return (1);
}
