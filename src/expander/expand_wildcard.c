#include "../../includes/minishell.h"

void	join_wild(char **arg, char *value, t_shell *shell)
{
	shell->is_wild = 1;
	join_word(arg, value, shell);
}

void	add_file(char ***args, char *file, t_shell *shell)
{
	char	*tmp;
	
	tmp = ft_strdup(file);
	if (*args == NULL)
	{
		*args = malloc(2 * sizeof(char *));
		if (*args == NULL || tmp == NULL)
		{
			shell->error = ERR_MEMORY;
			return ;
		}
		**args = tmp;
		*(*args + 1) = NULL;
		return ;
	}
	if (tmp == NULL)
	{
		shell->error = ERR_MEMORY;
		return ;
	}
	realloc_arr(args, tmp, shell);
}

int	match(char *file, char *arg)
{
	if (*arg == '*' && *(arg + 1) == '\0')
		return (1);
	while (*file && *arg)
	{
		while (*arg && *arg == '*')
			arg++;
		if (*arg && *file == *arg)
			arg++;
		file++;
	}
	if (*file == '\0' && *arg == '\0')
		return (1);
	return (0);
}

void	unmatched_wild(char *arg, t_shell *shell)
{
	ft_putstr_fd(ERR_WILDCARD_MSG, STD_ERR);
	ft_putendl_fd(arg, STD_ERR);
	shell->error = ERR_WILD;
}

void	expand_wild(char **args, char *arg, t_shell *shell)
{
	DIR				*dir;
	struct dirent	*obj;
	int				found;

	dir = opendir(".");
	obj = readdir(dir);
	found = 0;
	while (obj)
	{
		if (match(obj->d_name, arg))
		{
			add_file(args, obj->d_name, shell);
			if (shell->error)
			{
				closedir(dir);
				return ;
			}
			found++;
		}
		obj = readdir(dir);
	}
	closedir(dir);
	if (!found)
		unmatched_wild(arg, shell);
}
