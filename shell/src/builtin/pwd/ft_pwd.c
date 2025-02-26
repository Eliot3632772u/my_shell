#include "../../../includes/minishell.h"

char	*pwd(char *str)
{
	static char	*pwd;
	t_env		*env;
	t_env		*pwd_node;

	if (!str)
		return (pwd);
	env = get_envp(NULL);
	pwd_node = search_in_env(env, "PWD");
	if (pwd_node)
		set_env_value(ft_strdup("OLDPWD"), ft_strdup(pwd_node -> value), 1);
	else
		set_env_value(ft_strdup("OLDPWD"), ft_strdup(""), 1);
	free(pwd);
	pwd = trim_path_pwd(str);
	set_env_value(ft_strdup("PWD"), ft_strdup(pwd), 1);
	return (pwd);
}

void	shift_slice(char **slices)
{
	if (!slices && !*slices)
		return ;
	free(slices[0]);
	slices[0] = NULL;
	while (slices[1])
	{
		slices[0] = slices[1];
		slices++;
	}
	slices[0] = slices[1];
}

char	*trim_path_pwd(char *pwd)
{
	char	*result;
	char	**slices;
	char	**slice_ptr;
	size_t	i;

	if (!pwd)
		return (NULL);
	slices = ft_split(pwd, '/');
	slice_ptr = slices;
	i = 0;
	while (slice_ptr[i])
	{
		if (!ft_strcmp(slice_ptr[i], "."))
		{
			shift_slice(&slice_ptr[i]);
			slice_ptr = slices;
			continue ;
		}
		i++;
	}
	result = contruct_path(slices);
	return (free_list(slices), result);
}


int	ft_pwd(void)
{
	printf("%s\n", pwd(NULL));
	return (0);
}
