#include "../../includes/minishell.h"

// Frees all nodes in the environment linked list
void free_env(t_env *env)
{
    t_env *temp;

    while (env)
    {
        temp = env;
        env = env->next;
        free(temp->key);
        free(temp->value);
        free(temp);
    }
}

// Frees all strings in a string array and the array itself
void free_string_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
        free(array[i++]);
    free(array);
}

// Cleans up all dynamically allocated resources used by the shell
void cleanup_shell(t_shell *shell)
{
    if (shell->env)
        free_env(shell->env);
    if (shell->env_array)
        free_string_array(shell->env_array);
    if (shell->input)
        free(shell->input);
    // Command cleanup will be implemented later
}
