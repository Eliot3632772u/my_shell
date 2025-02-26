#include "../../includes/minishell.h"

void sigint_handler(int sig)
{
    (void)sig;
    if (waitpid(-1, &sig, WNOHANG) == 0)
        return ;
    printf("^C\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    set_exit_status(1);
    g_last_signal = 69;
}

void heredoc_sigint_handler(int sig)
{
    (void)sig;
    set_exit_status(1);
    g_last_signal = 420;
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_IGN);
}

void handle_default_sig_handlers(int action)
{
    if (action == SET) {
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT, SIG_IGN);
    } else {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
    }
}

void reset_default_sig_handlers(void)
{
    handle_default_sig_handlers(RESET);
}

void install_default_sig_handlers(void)
{
    handle_default_sig_handlers(SET);
}
