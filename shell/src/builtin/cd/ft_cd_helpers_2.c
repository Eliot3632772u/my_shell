/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_helpers_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 23:04:39 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/03 23:04:40 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	update_pwd(char *curr_pwd, char *new_pwd)
{
	if (curr_pwd && curr_pwd[0] != '/')
		free(curr_pwd);
	pwd(new_pwd);
	if (new_pwd)
		free(new_pwd);
}

int	handle_new_pwd(char *curr_pwd, char *dir)
{
	char	*new_pwd;

	new_pwd = resolve_new_pwd(curr_pwd, dir);
	update_pwd(curr_pwd, new_pwd);
	return (0);
}

int	change_directory(char *dir)
{
	char	*curr_pwd;
	int		chdir_result;

	if (!dir || !*dir)
	{
		ft_putstr_fd("cd: No such file or directory\n", 2);
		return (1);
	}
	curr_pwd = pwd(NULL);
	if (!curr_pwd)
		curr_pwd = ft_strdup("/");
	chdir_result = chdir(dir);
	if (chdir_result == -1)
	{
		perror("cd");
		if (curr_pwd && curr_pwd[0] != '/')
			free(curr_pwd);
		return (1);
	}
	return (handle_new_pwd(curr_pwd, dir));
}
