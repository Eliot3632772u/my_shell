#include "../../includes/minishell.h"

unsigned int get_chunk_len(char *ptr, char *sp)
{
    unsigned int len;

    len = 0;
    if (!ft_strchr(sp, *ptr))
    {
        while (ptr[len] && !ft_strchr(sp, ptr[len]))
            len++;
    }
    else
    {
        len++;
        if (ptr[1] == '?' || ptr[1] == '*' || ptr[1] == '$')
            return (len + 1);
        if (ptr[1] && ft_isdigit(ptr[1]))
            return (len + 1);
        while (ft_isalnum(ptr[len]) || ptr[len] == '_')
            len++;
    }
    return (len);
}

void ghost_char(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == ' ' || str[i] == '\t')
            str[i] = (char)TROLL;
        i++;
    }
}
