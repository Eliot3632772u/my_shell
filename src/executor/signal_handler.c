#include "../../includes/minishell.h"

void handle_signal(int signo)
{
    if (signo == SIGINT)  // Ctrl+C
    {
        g_signal_received = SIGINT;
        ft_printf("\n"); // Print a newline
        rl_on_new_line(); // Notify readline of a new line
        //rl_replace_line("", 0); // Clear the current line
        rl_redisplay(); // Redisplay the prompt
    }
}

void setup_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    sigaction(SIGINT, &sa, NULL);   // Handle Ctrl+C
    signal(SIGQUIT, SIG_IGN);       /* Ignore Ctrl+\ */
}

// Resets signal handling to default behaviors
void reset_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}
