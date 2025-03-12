/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:39:51 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/11 11:03:16 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	process_heredoc_file(char *file_name, t_strbuilder *sb, \
	char **full_path)
{
	*full_path = NULL;
	if (ft_strncmp(file_name, ".Minishell_HEREDOC_", 19) == 0)
	{
		sb_append(sb, "/tmp/");
		sb_append(sb, file_name);
		*full_path = ft_strdup(sb->str);
		if (*full_path && access(*full_path, F_OK) == 0)
			unlink(*full_path);
		if (*full_path)
			free(*full_path);
	}
}

void	cleanup_heredoc_file(char *tmp_file)
{
	if (tmp_file)
	{
		if (access(tmp_file, F_OK) == 0)
			unlink(tmp_file);
		free(tmp_file);
	}
}

static void	process_heredoc_entries(DIR *dir)
{
	struct dirent	*entry;
	char			*full_path;
	t_strbuilder	*sb;

	entry = readdir(dir);
	while (entry)
	{
		sb = stringbuilder();
		if (sb)
		{
			process_heredoc_file(entry->d_name, sb, &full_path);
			sb_free(sb);
		}
		entry = readdir(dir);
	}
}

void	cleanup_all_heredoc_files(void)
{
	DIR		*dir;

	dir = opendir("/tmp");
	if (!dir)
		return ;
	process_heredoc_entries(dir);
	closedir(dir);
}
