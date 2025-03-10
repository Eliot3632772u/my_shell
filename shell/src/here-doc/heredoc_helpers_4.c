/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers_4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:39:51 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/10 10:41:48 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_heredoc_file(char *tmp_file)
{
	if (tmp_file)
	{
		if (access(tmp_file, F_OK) == 0)
			unlink(tmp_file);
		free(tmp_file);
	}
}

void	cleanup_all_heredoc_files(void)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*full_path;
	t_strbuilder	*sb;

	dir = opendir("/tmp");
	if (!dir)
		return ;
	entry = readdir(dir);
	while (entry)
	{
		if (ft_strncmp(entry->d_name, ".Minishell_HEREDOC_", 19) == 0)
		{
			sb = stringbuilder();
			sb_append(sb, "/tmp/");
			sb_append(sb, entry->d_name);
			full_path = ft_strdup(sb->str);
			sb_free(sb);
			if (full_path && access(full_path, F_OK) == 0)
				unlink(full_path);
			free(full_path);
		}
		entry = readdir(dir);
	}
	closedir(dir);
}
