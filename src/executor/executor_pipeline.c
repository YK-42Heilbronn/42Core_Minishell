/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:21:38 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/11 17:54:51 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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

int run_child(t_shell *shell, t_cmd *cmd, int in_fd, int out_fd)
{
    char *path;
    
    
}