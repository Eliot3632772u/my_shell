#include "../../includes/minishell.h"

void	free_arr(char **arr)
{
	int		i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	count_strs(char **args)
{
	int		i;

	i = 0;
	while(args[i])
		i++;
	return (i);
}

void	realloc_arr(char ***args, char *arg, t_shell *shell)
{
	int		i;
	char	**new;

	i = count_strs(*args);
	new = malloc((i + 2) * sizeof(char *));
	if (new == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	new[i + 1] = NULL;
	i = 0;
	while ((*args)[i])
	{
		new[i] = (*args)[i];
		i++;
	}
	new[i] = arg;
	printf("      %s     \n\n", new[i]);
	free(*args);
	*args = new;
}

void	join_word(char **arg, char *value, t_shell *shell)
{
	char	*tmp;

	if (*arg == NULL)
	{
		*arg = ft_strdup(value);
		if (*arg == NULL)
		{
			shell->error = ERR_MEMORY;
			return ;
		}
		return ;
	}
	tmp = *arg;
	*arg = ft_strjoin(*arg, value);
	free(tmp);
	if (*arg == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
}
