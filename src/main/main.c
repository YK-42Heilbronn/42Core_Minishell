/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:00:22 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/10 23:23:23 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int main(int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    init_shell(&shell, envp);
    setup_signals();
    shell_loop(&shell);
    cleanup_shell(&shell);
    return(shell->last_status);
}

static int shell_loop(t_shell *shell)
{
    while(1)
    {
        shell->line = readline("minishell$ ");
        if (!shell->line)
        {
            write(1, "exit\n", 5);
            break ;
        }
        if (shell->line[0] != '\0')
            add_history(shell->line);
        if(process_line(&shell) == -1)
        {
            free(shell->line);
            shell->line = NULL;
            continue;
        }
        free(shell->line);
        shell->line = NULL;
        cleanup_command(shell);
    }
    return(0);
}
