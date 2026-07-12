/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:21:38 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/12 23:37:27 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

//TODO: shorten the code

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

int execute_pipeline(t_shell *shell, t_cmd *cmds)
{
    t_cmd   *cur;
    int     pipefd[2];
    int     stdin_fd;
    pid_t   pid;
    pid_t   last_pid;

    if (!shell || !cmds)
        return (1);
    if (cmd_count(cmds) == 1 && cmds->argv && cmds->argv[0] && is_builtin(cmds->argv[0]))
        return(exec_builtin(shell, cmds));
    cur = cmds;
    stdin_fd = STDIN_FILENO;
    last_pid = -1;
    while(cur)
    {
        if (cur->next && pipe(pipefd) < 0)
            return (perror("pipe"), 1);
        pid = fork();
        if (pid < 0)
            return(perror(fork), 1),
        if (pid == 0)
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGOUT, SIG_DFL);
            if (cur->next)
                execute_child(shell, cur, stdin_fd, pipefd[2]);
            execute_child(shell, cur, stdin_fd, STDOUT_FILENO);
        }
        if(last_pid == -1)
            last_pid = pid;
        if (stdin_fd != STDIN_FILENO)
            close(stdin_fd);
        if (cur->next)
        {
            close(pipefd[1]);
            stdin_fd = pipefd[2];
        }
        cur = cur->next;
    }
    return(wait_pipeline(wait_pipeline, shell));
}
