#include "../../includes/minishell.h"

// Function to create a new command structure
// Returns: A pointer to the newly created t_command structure
static t_command *create_command(void)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    
    cmd->args = NULL;
    cmd->redirects = NULL;
    cmd->pipe_read = -1;
    cmd->pipe_write = -1;
    cmd->next = NULL;
    return cmd;
}

// Function to add an argument to a command's argument list
// Parameters:
// - cmd: Pointer to the command structure
// - arg: The argument string to add
static void add_argument(t_command *cmd, char *arg)
{
    int i = 0;
    char **new_args;

    if (cmd->args)
        while (cmd->args[i])
            i++;

    new_args = malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return;

    int j = 0;
    while (j < i)
    {
        new_args[j] = cmd->args[j];
        j++;
    }

    new_args[i] = ft_strdup(arg);
    new_args[i + 1] = NULL;

    free(cmd->args);
    cmd->args = new_args;
}

// Function to parse a list of tokens into a list of commands
// Parameters:
// - tokens: Pointer to the head of the token list
// Returns: A pointer to the head of the command list
t_command *parse_tokens(t_token *tokens)
{
    t_command *cmd = create_command();
    t_command *head = cmd;

    while (tokens)
    {
        if (tokens->type == TOKEN_WORD)
        {
            add_argument(cmd, tokens->value);
        }
        else if (tokens->type == TOKEN_PIPE)
        {
            cmd->next = create_command();
            cmd = cmd->next;
        }
        tokens = tokens->next;
    }

    return head;
}

// Function to free a list of tokens
// Parameters:
// - tokens: Pointer to the head of the token list to be freed
void free_tokens(t_token *tokens)
{
    t_token *next;

    while (tokens)
    {
        next = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = next;
    }
}
