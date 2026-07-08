/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:00:22 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/09 00:03:10 by ileongar         ###   ########.fr       */
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

    while(1)
    {
        shell->line = readline("minishell$ ");
        if (!shell->line)
            break;
        if (shell->line[0] != '\0')
            add_history(shell->line);
        if(process_line(&shell) == -1)
        {
            free(shell->line);
            continue;
        }
        free(shell->line);
        cleanup_command(&shell);
    }
    cleanup_shell(&shell);
    return(shell->exit_status);
}
