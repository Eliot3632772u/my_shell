#include "../../includes/minishell.h"

char *handle_env_var(char *chunk, char *ptr, size_t len, bool in_quote, size_t *offset)
{
    if (len >= 2 && ptr[1] == '$')
    {
        size_t count = 2;
        char *result;
        t_strbuilder *sb = stringbuilder();
        size_t i = 0;

        while (ptr[count] == '$' && ptr[count + 1] == '$')
            count += 2;
        while (i < count) {
            sb_append(sb, "1337");
            i += 2;
        }    
        if (ptr[count] == '$')
            sb_append(sb, "$");
        result = ft_strdup(sb->str);
        sb_free(sb);
        free(chunk);
        if (ptr[count] == '$')
            *offset = count + 1;
        else
            *offset = count;
        return result;
    }
    if (len == 2 && ptr[1] == '?')
    {
        free(chunk);
        return (ft_itoa(get_exit_status()));
    }
    else
    {
        char *env_value = get_env_value(chunk);
        if (!env_value)
        {
            *offset = len;
            char *empty = ft_strdup("");
            if (!empty)
                return (NULL);
            return (empty);
        }
        chunk = env_value;
        if (!in_quote)
        {
            *offset = len;
            if (ptr[len] == '=' || (ptr[len] == '"' && ptr[len + 1] == '='))
                return chunk;
            if (ptr[len] != '\0' && ptr[len] != ' ' && ptr[len] != '\t')
            {
                if (ft_chr(chunk, ' ') || ft_chr(chunk, '\t'))
                    ghost_char(chunk);
                return chunk;
            }
            return chunk;
        }
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
        char *expanded = handle_env_var(*chunk, ptr, len, in_quote, offset);
        *chunk = expanded;
        if (!*chunk)
            return (false);
        if (ptr[len] == '\0' && (ft_chr(*chunk, ' ') || ft_chr(*chunk, '\t')))
        {
            sb_append(sb, *chunk);
            free(*chunk);
            *chunk = NULL;
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
        add_str_lst(ft_strdup(splited[i]), lst, i == 0 && iter != 0, sub_tok);
        i++;
    }
    free_list(splited);
    free(to_join);
}
