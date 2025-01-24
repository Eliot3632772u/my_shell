#include "../../includes/minishell.h"

int g_signal_received = 0;

static void init_shell(t_shell *shell, char **envp)
{
    shell->env = init_env(envp);
    shell->cmd = NULL;
    shell->input = NULL;
    shell->last_status = 0;
    shell->running = 1;
    shell->env_array = NULL;
    shell->error = 0;
    //shell->is_DOLLAR = 0;
}

char    *f(t_token_type type)
{
    if (type == WORD)
        return (ft_strdup("WORD"));
    else if (type == WORD_VAR)
        return (ft_strdup("WORD_VAR"));
    else if (type == REDIRECT_IN)
        return (ft_strdup("REDIRECT_IN"));
    else if (type == REDIRECT_OUT)
        return (ft_strdup("REDIRECT_OUT"));
    else if (type == APPEND)
        return (ft_strdup("APPEND"));
    else if (type == HEREDOC)
        return (ft_strdup("HERDOC"));
    else if (type == T_AND)
        return (ft_strdup("TOKEN_AND"));
    else if (type == T_OR)
        return (ft_strdup("TOKEN_OR"));
    else if (type == WILD)
        return (ft_strdup("WILDCARD"));
    if (type == VARIABL)
        return (ft_strdup("VARIABLE"));
    if (type == O_P)
        return (ft_strdup("OPEN BRACKET"));
    if (type == C_P)
        return (ft_strdup("CLOSE BRACKET"));
    if (type == PIPE)
        return (ft_strdup("PIPE"));
    return (NULL);
}

int main(int argc, char **argv, char **envp)
{
    t_shell shell;
    t_token *token;
    char *input;
    (void)argc;
    (void)argv;

    // Initialize shell
    init_shell(&shell, envp);
    //setup_signals();

    token = NULL;
    // Main shell loop
    while (shell.running)
    {
        input = readline("minishell$ ");
        if (!input)  // Handle Ctrl+D (EOF)
        {
            ft_printf("exit\n");
            break;
        }
        if (input[0] != '\0')
        {
            add_history(input);
            // Here the parser will be integrated
            // i'll implement basic command handling
            shell.input = input;
            lexer(&shell, &token, input);
            if (shell.error)
            {
                free_tokens(token);
                token = NULL;
                free(shell.input);
                shell.input = NULL;
                shell.error = 0;
                continue;
            }
            while (token)
            {
                char *s = f(token->type);
                printf("%s --> %s", token->value, s);
                if (token->concate)
                    printf("🔗🔗🔗🔗");
                printf("\n");
                token = token->next;
            }
            // Execute command (i'll implement this next)
            // execute_command(shell.cmd, &shell);
        }
    }

    //cleanup_shell(&shell);
    return (0/*shell.last_status*/);
}
