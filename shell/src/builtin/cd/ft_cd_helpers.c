/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:04:46 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/03 23:04:47 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*join_path_with_dir(char *curr_pwd, char *dir)
{
	char	*tmp;
	char	*new_pwd;

	tmp = ft_strjoin(curr_pwd, "/");
	if (!tmp)
		return (NULL);
	new_pwd = ft_strjoin(tmp, dir);
	free(tmp);
	return (new_pwd);
}

char	*construct_manual_path(char *dir, char *curr_pwd)
{
	if (ft_strcmp(dir, "..") == 0)
		return (join_path_with_dir(curr_pwd, ".."));
	else if (ft_strcmp(dir, ".") == 0)
		return (join_path_with_dir(curr_pwd, "."));
	else
		return (join_path_with_dir(curr_pwd, dir));
}

char	*handle_fallback_case(char *dir, char *curr_pwd)
{
	char	*new_pwd;
	char	*tmp;

	if (ft_strcmp(dir, ".") == 0)
		return (ft_strdup(curr_pwd));
	else
	{
		tmp = ft_strjoin(curr_pwd, "/");
		if (tmp)
			new_pwd = ft_strjoin(tmp, dir);
		else
			new_pwd = NULL;
		free(tmp);
		return (new_pwd);
	}
}

char	*get_new_pwd(char *dir, char *curr_pwd)
{
	char	*cwd;
	char	*new_pwd;

	if (!curr_pwd)
		return (NULL);
	if (dir[0] == '/')
		return (ft_strdup(dir));
	cwd = getcwd(NULL, 0);
	if (cwd == NULL && errno == ENOENT)
		return (construct_manual_path(dir, curr_pwd));
	else if (cwd)
	{
		new_pwd = ft_strdup(cwd);
		free(cwd);
		return (new_pwd);
	}
	return (handle_fallback_case(dir, curr_pwd));
}

char	*resolve_new_pwd(char *curr_pwd, char *dir)
{
	char	*temp_pwd;
	char	*new_pwd;
	int		saved_errno;

	temp_pwd = getcwd(NULL, 0);
	if (!temp_pwd)
	{
		saved_errno = errno;
		if (errno == ENOENT)
		{
			perror("cd: error retrieving current directory: getcwd");
			new_pwd = get_new_pwd(dir, curr_pwd);
		}
		else
		{
			new_pwd = ft_strdup(curr_pwd);
		}
		errno = saved_errno;
	}
	else
	{
		new_pwd = ft_strdup(temp_pwd);
		free(temp_pwd);
	}
	return (new_pwd);
}
