#include "../../../includes/minishell.h"

char *trim_path(char *path)
{
    char **parts;
    char *result;

    if (!path)
        return (NULL);
    parts = ft_split(path, '/');
    if (!parts)
        return (NULL);
    
    parts = handle_dot_dot_path(ft_strdup(path));
    if (!parts)
        return (NULL);

    result = contruct_path(parts);
    free_list(parts);

    return (result);
}
