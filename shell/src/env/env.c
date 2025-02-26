#include "../../includes/minishell.h"

t_env *search_in_env(t_env *env, char *key)
{
    t_env *ptr;

    if (!env || !key)
        return (NULL);
    ptr = env;
    while (ptr)
    {
        if (!ft_strcmp(ptr->key, key))
            return (ptr);
        ptr = ptr->next;
    }
    return (NULL);
}

t_env **get_envp_internal(t_env *envp)
{
    static t_env *env;

    if (envp)
        env = envp;
    return (&env);
}

t_env *get_envp(t_env *envp)
{
    if (envp)
        get_envp_internal(envp);
    return (*get_envp_internal(NULL));
}

static void sort_env_array(char **array, size_t size)
{
    size_t i;
    size_t j;
    char *temp;

    i = 0;
    while (i < size - 1)
    {
        j = 0;
        while (j < size - i - 1)
        {
            if (ft_strcmp(array[j], array[j + 1]) > 0)
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
            j++;
        }
        i++;
    }
}

char **consume_env(t_env *env)
{
    size_t i;
    char **ptr;
    size_t size;
    t_env *node;
    char *tmp;

    if (!env)
        return (NULL);
    size = ft_lstsize2(env);
    ptr = (char **)ft_malloc((size + 1) * sizeof(char *));
    node = env;
    i = 0;
    while (i < size)
    {
        tmp = ft_strjoin(node->key, "=");
        ptr[i++] = ft_strjoin(tmp, node->value);
        free(tmp);
        node = node->next;
    }
    ptr[i] = NULL;
    sort_env_array(ptr, size);
    return (ptr);
}

char *get_env_value(char *var)
{
    t_env *node;

    if (!var)
        return (NULL);
    node = search_in_env(get_envp(NULL), var + 1);
    free(var);
    if (!node || !node->value)
        return (NULL);
    return (ft_strdup(node->value));
}
