/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:00:22 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 00:26:47 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

// static int	shell_loop(t_shell *shell);

// int	main(int argc, char **argv, char **envp)
// {
// 	t_shell	shell;

// 	(void)argc;
// 	(void)argv;
// 	// ft_memset(&shell, 0, sizeof(shell));
// 	init_shell(&shell, envp);
// 	setup_signals();
// 	shell_loop(&shell);
// 	// cleanup_shell(&shell);
// 	return (shell.last_status);
// }

// static int shell_loop(t_shell *shell)
// {
//     while (1)
//     {
//         shell->line = readline("minishell$ ");
//         if (!shell->line)
//         {
//             write(1, "exit\n", 5);
//             break;
//         }
//         if (shell->line[0] != '\0')
//             add_history(shell->line);
//         if(process_line(shell) == -1)
//         {
//             free(shell->line);
//             shell->line = NULL;
//             continue;
//         }
//         free(shell->line);
//         shell->line = NULL;
//         if (shell->exit_flag)
//             break;
//         cleanup_command(shell);
//     }
//     return (0);
// }

static int shell_loop(t_shell *shell);

int main(int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;

    ft_memset(&shell, 0, sizeof(shell));
    init_shell(&shell, envp);
    setup_signals();

    shell_loop(&shell);

    cleanup_command(&shell);
    return shell.last_status;
}

static int shell_loop(t_shell *shell)
{
    int ret;

    while (1)
    {
        shell->line = readline("minishell$ ");
        if (!shell->line)
        {
            write(1, "exit\n", 5);
            break;
        }

        if (shell->line[0] != '\0')
            add_history(shell->line);

        ret = process_line(shell);
        if (ret == -1)
        {
            /* Error in parsing/expansion/execution setup */
            free(shell->line);
            shell->line = NULL;
            cleanup_command(shell);
            continue;
        }

        free(shell->line);
        shell->line = NULL;

        if (shell->exit_flag)
            break;
    }
    return 0;
}

int process_line(t_shell *shell)
{
    int ret;

    ret = process_input(shell->line, shell);
    if (ret == -1)
        return -1;  /* propagate error */

    shell->last_status = execute_shell(shell);
    return shell->last_status;
}
