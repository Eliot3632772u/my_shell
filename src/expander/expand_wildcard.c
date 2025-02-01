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

int is_special_case(char *file)
{
    return (ft_strcmp(file, ".") == 0 || ft_strcmp(file, "..") == 0);
}
void	increment(char **arg, char **file)
{
	(*arg)++;
	(*file)++;
}

int match(char *file, char *arg)
{
    char *star = NULL;
    char *file_pos = NULL;

    while (*file)
    {
        if (*arg == '*')
        {
            star = arg++;
            file_pos = file;
        }
        else if (*arg == *file)
			increment(&arg, &file);
        else if (star)
        {
            arg = star + 1;
            file = ++file_pos;
        }
        else
            return (0);
    }
    while (*arg == '*')
        arg++;
    return (*arg == '\0');
}

void	unmatched_wild(char *arg, t_shell *shell)
{
	ft_putstr_fd(ERR_WILDCARD_MSG, STD_ERR);
	ft_putendl_fd(arg, STD_ERR);
	shell->error = ERR_WILD;
}

void	expand_wild(char ***args, char *arg, t_shell *shell)
{
	DIR				*dir;
	struct dirent	*obj;
	int				found;

	dir = opendir(".");
	obj = readdir(dir);
	found = 0;
	while (obj)
	{
		if (!is_special_case(obj->d_name) && match(obj->d_name, arg))
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
