/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 23:45:02 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/12 00:10:55 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"
#include "../include/parser.h"

int executor_child(t_shell *shell, t_cmd *cmd, int stdin_fd, int stdout_fd)
{
    char *path;
    
    if (stdin_fd != STDIN_FILENO)
    {
        dup2(stdin_fd, STDIN_FILENO);
        close(stdin_fd);
    }
    if (stdout_fd != STDOUT_FILENO)
    {
        dup2(stdout_fd, STDOUT_FILENO);
        close(stdout_fd);
    }
    if (apply_redirs(shell, cmd, 1))
        exit(1);
    if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
        exit(exec_builtin(shell, cmd));
    path = resolve_cmd_path(shell, cmd->argv[0]);
    if(!path)
        exit(127);
    execve(path, cmd->argv, NULL);
    free(path);
    exit(127);
}

// TODO: dup2() --> check closer

apply_redirs()

is_builtin()

exec_builtin()

resolve_cmd_path()