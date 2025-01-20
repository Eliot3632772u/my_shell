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
    setup_signals();

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
            lex(&token, input, &shell);
            shell.input = input;
            // Execute command (i'll implement this next)
            // execute_command(shell.cmd, &shell);
        }
    }

    cleanup_shell(&shell);
    return (shell.last_status);
}
