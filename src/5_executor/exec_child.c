/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 23:45:02 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/13 09:37:45 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"

int open_redir_fd(t_shell *shell, t_redir *r);
int apply_redirs(t_shell *shell, t_cmd *cmd, int is_child);

int execute_child(t_shell *shell, t_cmd *cmd, int stdin_fd, int stdout_fd)
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
    path = cmd_path_resolution(shell, cmd->argv[0]);
    if(!path)
        exit(127);
    execve(path, cmd->argv, NULL);
    free(path);
    exit(127);
}

int open_redir_fd(t_shell *shell, t_redir *r)
{
    if (r->type == R_HEREDOC)
        return (shell->heredoc_fd);
    if (r->type == R_IN)
        return (open(r->file, O_RDONLY));
    if (r->type == R_OUT)
        return (open(r->file, O_WRONLY | O_CREAT | O_TRUNC, 0644));
    if (r->type == R_APPEND)
        return (open(r->file, O_WRONLY | O_CREAT | O_APPEND, 0644));
    return (-1);
}

int apply_redirs(t_shell *shell, t_cmd *cmd, int is_child)
{
    t_redir *r;
    int     fd;

    (void)is_child;
    if (!shell || cmd)
        return (1);
    r = cmd->redirs;
    while (r)
    {
        fd = open_redir_fd(shell, r);
        if (fd < 0)
        {
            perror(r->file);
            return (1);
        }
        if (r->type == R_HEREDOC || r->type == R_IN)
        {
            if (dup2(fd, STDIN_FILENO) < 0)
            {
                perror("dup2");
                if (r->type != R_HEREDOC)
                    close(fd);
                return (1);
            }
            if (r->type != R_HEREDOC)
                close(fd);
        }
        else
        {
            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                perror(strerror(dup2(fd, STDOUT_FILENO)));
                close(fd);
                return(1);
            }
            close(fd);
        }
        r = r->next;
    }
    return (0);
}
