/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:21:38 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/12 01:20:58 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"

int wait_pipeline(pid_t last_pid, t_shell *shell)
{
    int     status;
    int     last_status;
    pid_t   pid;

    last_status = 0;
    pid = wait(&status);
    while(pid > 0)
    {
        if (pid == last_pid)
            last_status = status;
        pid = wait(&status);
    }
    if (WIFEXITED(last_status))
        shell->last_status = WEXITSTATUS(last_status);
    else if(WIFSIGNALED(last_status))
        shell->last_status = 128 + WTERMSIG(last_status);
    else
        shell->last_status = 1;
    return(shell->last_status);
}

// int execute_pipeline(t_shell *shell, t_cmd *cmds)
// {
//     t_cmd   *cur;
//     int     pipefd[2];
//     int     stdin_fd;
//     pid_t   pid;
//     pid_t   last_pid;

//     if (!shell || !cmds)
//         return (1);
//     if (cmd_count(cmds) == 1 && cmds->argv && cmds->argv[0] && is_builtin(cmds->argv[0]))
//         return(exec_builtin(shell, cmds));
//     cur = cmds;
//     stdin_fd = STDIN_FILENO;
//     last_pid = -1;
//     while(cur)
//     {
//         if(cur->next)
//     }
// }
