/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:59:45 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/07 22:53:07 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	get_username(char *username)
{
	char	*env_user;

	env_user = getenv("USER");
	if (!env_user)
		env_user = getenv("LOGNAME");
	if (env_user)
		ft_strlcpy(username, env_user, 256);
	else
		ft_strlcpy(username, "unknown", 256);
}

void	get_hostname(char *hostname)
{
	int		fd;
	char	*line;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd >= 0)
	{
		line = get_next_line(fd);
		close(fd);
		if (line)
		{
			ft_strlcpy(hostname, line, 256);
			if (hostname[ft_strlen(hostname) - 1] == '\n')
				hostname[ft_strlen(hostname) - 1] = '\0';
			free(line);
		}
		else
			ft_strlcpy(hostname, "unknown", 256);
	}
	else
		ft_strlcpy(hostname, "unknown", 256);
}

void	get_system_info(char *username, char *hostname)
{
	get_username(username);
	get_hostname(hostname);
}

char	*build_prompt_start(void)
{
	char	username[256];
	char	hostname[256];
	char	*start;
	char	*temp;

	get_system_info(username, hostname);
	start = ft_strjoin(COLOR_CYAN "┌──(" COLOR_RED, username);
	temp = start;
	start = ft_strjoin(temp, COLOR_CYAN "㉿" COLOR_RED);
	free(temp);
	temp = start;
	start = ft_strjoin(temp, hostname);
	free(temp);
	temp = start;
	start = ft_strjoin(temp, COLOR_CYAN ")-[" COLOR_BLUE);
	free(temp);
	return (start);
}

char	*get_formatted_path(void)
{
	char	*pwd_value;
	char	*home;
	char	*path;
	char	*result;

	pwd_value = pwd(NULL);
	if (!pwd_value)
	{
		pwd_value = getcwd(NULL, 0);
		if (!pwd_value)
			return (ft_strdup("unknown"));
	}
	else
		pwd_value = ft_strdup(pwd_value);
	home = getenv("HOME");
	if (home && ft_strncmp(pwd_value, home, ft_strlen(home)) == 0)
	{
		path = pwd_value + ft_strlen(home);
		result = ft_strjoin("~", path);
		free(pwd_value);
		return (result);
	}
	return (pwd_value);
}
