#include "../../../includes/minishell.h"

int	ft_cd(char *arg, t_env *env)
{
	t_env	*node;

	if (!arg)
	{
		node = search_in_env(env, "HOME");
		if (!node)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
		else
			return (change_directory(node->value));
	}
	else
		return (change_directory(arg));
}
