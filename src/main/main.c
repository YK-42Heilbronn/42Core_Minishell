/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:00:22 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/08 21:45:25 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int main (int argc, char **argv, char **envp)
{
    t_shell sh;
    
    (void)argc;
    (void)argv;
    shell_init(&sh, envp);
    setup_signals();

    while(1)
    {
        sh->line = readline("minishell$ ");
        if (!sh->line)
            break;
        if (sh->line[0] != '\0')
            add_history(sh->line);
        if(process_line(&sh) == -1)
        {
            free(sh->line);
            continue;
        }
        free(sh->line);
        cleanup_command(&sh);
    }
    cleanup_shell(&sh);
    return(sh->exit_status);
}
