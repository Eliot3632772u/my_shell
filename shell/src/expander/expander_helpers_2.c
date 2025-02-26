#include "../../includes/minishell.h"

char *handle_env_var(char *chunk, char *ptr, size_t len, bool in_quote, size_t *offset)
{
    if (len == 2 && ptr[1] == '?')
    {
        free(chunk);
        chunk = ft_itoa(get_exit_status());
    }
    else
    {
        chunk = get_env_value(chunk);
        if (!chunk)
        {
            *offset = len;
            return (NULL);
        }
        if (!in_quote && ptr[len] == '\0' && (ft_chr(chunk, ' ') || ft_chr(chunk, '\t')))
        {
            *offset = len;
            return (chunk);
        }
        else if (!in_quote && (ft_chr(chunk, ' ') || ft_chr(chunk, '\t')))
            ghost_char(chunk);
    }
    return (chunk);
}

bool handle_chunk(t_strbuilder *sb, char **chunk, char *ptr, 
    bool in_quote, bool ignore_env, size_t *offset)
{
    size_t len;

    len = get_chunk_len(ptr, "$");
    *chunk = ft_substr(ptr, 0, len);
    *offset = len;
    if (!ignore_env && *ptr == '$' && len > 1)
    {
        *chunk = handle_env_var(*chunk, ptr, len, in_quote, offset);
        if (!*chunk)
            return (false);
        if (ptr[len] == '\0' && (ft_chr(*chunk, ' ') || ft_chr(*chunk, '\t')))
        {
            sb_append_free(sb, *chunk);
            return (false);
        }
    }
    return (true);
}

void handle_split_args(char *to_join, t_str **lst, int iter, t_token *sub_tok, char split_char)
{
    char **splited;
    int i;

    splited = ft_split(to_join, split_char);
    i = 0;
    while (splited[i])
    {
        add_str_lst(splited[i], lst, i == 0 && iter != 0, sub_tok);
        i++;
    }
    (free_list(splited), splited = NULL);
}