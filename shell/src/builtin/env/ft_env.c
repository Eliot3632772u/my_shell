#include "../../../includes/minishell.h"

int	print_env(t_env *env)
{
	t_env	*ptr;

	if (!env)
		return (1);
	ptr = env;
	while (ptr)
	{
		if (ptr->equal_sign && ptr->value)
			printf("%s=%s\n", ptr->key, ptr->value);
		ptr = ptr->next;
	}
	return (0);
}

int	ft_env(t_env **env)
{
	return (print_env(*env));
}
