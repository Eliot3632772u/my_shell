#include "../../includes/minishell.h"

// Frees the entire linked list of environment variables
t_env *init_env(char **envp)
{
    t_env *env_list;
    t_env *new_node;
    char *equals_pos;
    int i;

    env_list = NULL;
    i = 0;
    while (envp[i]) // Iterate through the environment variables array
    {
        equals_pos = ft_strchr(envp[i], '='); // Find '=' in the string
        if (equals_pos)
        {
            new_node = malloc(sizeof(t_env));
            if (!new_node)
                return (NULL);
            new_node->key = ft_substr(envp[i], 0, equals_pos - envp[i]); // Extract the key
            new_node->value = ft_strdup(equals_pos + 1); // Extract the value
            new_node->next = env_list; // Link the new node to the list
            env_list = new_node;
        }
        i++;
    }
    return (env_list); // Return the head of the linked list
}

// Retrieves the value associated with a given key in the environment list
char *get_env_value(t_env *env, const char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0) // Compare the key with the target
            return (env->value); // Return the matching value
        env = env->next;
    }
    return (NULL);
}


// Updates the value of an existing environment variable or does nothing if the key is not found
void set_env_value(t_env *env, const char *key, const char *value)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0) // Check for a matching key
        {
            free(env->value); // Free the existing value
            env->value = ft_strdup(value); // Assign the new value
            return;
        }
        env = env->next;
    }
}

// Converts the environment linked list into an array of "key=value" strings
char **env_to_array(t_env *env)
{
    int count;
    t_env *temp;
    char **array;
    char *tmp_str;
    int i;

    count = 0;
    temp = env;
    while (temp && ++count) // Count the number of nodes in the linked list
        temp = temp->next;

    array = malloc(sizeof(char *) * (count + 1));
    if (!array)
        return (NULL);

    i = 0;
    while (env) // Convert each node to "key=value" format
    {
        tmp_str = ft_strjoin(env->key, "="); // Create "key="
        array[i] = ft_strjoin(tmp_str, env->value); // Append the value
        free(tmp_str);
        env = env->next;
        i++;
    }
    array[i] = NULL;
    return (array);
}