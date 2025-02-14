/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-02-03 09:26:38 by irabhi            #+#    #+#             */
/*   Updated: 2025-02-03 09:26:38 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	char	*star;

	star = NULL;
	while (*file)
	{
		if (*arg == '*')
			star = arg++;
		else if (*arg == *file)
			increment(&arg, &file);
		else if (star)
		{
			arg = star + 1;
			file++;
		}
		else
			return (0);
	}
	while (*arg == '*')
		arg++;
	return (*arg == '\0');
}

int	get_file(struct dirent **obj, DIR *dir, t_shell *shell)
{
	*obj = readdir(dir);
	if (errno)
	{
		shell->error = RD_DIR_F;
		return (1);
	}
	return (0);
}

void	expand_wild(char ***args, char *arg, t_shell *shell)
{
	DIR				*dir;
	struct dirent	*obj;
	int				found;

	
	if (init_dir(&obj, &dir, shell))
		return ;
	found = 0;
	while (obj)
	{
		if (!is_special_case(obj->d_name, arg) && match(obj->d_name, arg))
		{
			add_file(args, obj->d_name, shell);
			if (shell->error)
				return (void)closedir(dir);
			found++;
		}
		if (get_file(&obj, dir, shell))
			return (void)closedir(dir);
	}
	closedir(dir);
	if (!found)
		unmatched_wild(arg, shell);
}
