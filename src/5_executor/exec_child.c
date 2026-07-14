/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 23:45:02 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 00:25:51 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"
#include "expander.h"

// int	run_child_process(t_shell *shell, t_cmd *cmd, int stdin_fd, int stdout_fd)
// {
// 	char	*path;

// 	if (stdin_fd != STDIN_FILENO)
// 	{
// 		dup2(stdin_fd, STDIN_FILENO);
// 		close(stdin_fd);
// 	}
// 	if (stdout_fd != STDOUT_FILENO)
// 	{
// 		dup2(stdout_fd, STDOUT_FILENO);
// 		close(stdout_fd);
// 	}
// 	if (apply_redirs(shell, cmd, 1))
// 		exit(1);
// 	if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
// 		exit(exec_builtin(shell, cmd));
// 	path = cmd_path_resolution(shell, cmd->argv[0]);
// 	if (!path)
// 		_exit(127);
// 	execve(path, cmd->argv, NULL);
// 	free(path);
// 	_exit(127);
// }

int run_child_process(t_shell *shell, t_cmd *cmd, int stdin_fd, int stdout_fd)
{
    char    *path;
    char    **envp;

    if (stdin_fd != STDIN_FILENO)
    {
        if (dup2(stdin_fd, STDIN_FILENO) < 0)
            _exit(1);
        close(stdin_fd);
    }
    if (stdout_fd != STDOUT_FILENO)
    {
        if (dup2(stdout_fd, STDOUT_FILENO) < 0)
            _exit(1);
        close(stdout_fd);
    }
    if (apply_redirs(shell, cmd, 1))
        _exit(1);
    if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
        _exit(exec_builtin(shell, cmd));
    path = cmd_path_resolution(shell, cmd->argv[0]);
    if (!path)
    {
        perror(cmd->argv[0]);
        _exit(127);
    }
    envp = env_to_array(shell->env);
    if (!envp)
    {
        free(path);
        _exit(1);
    }
    execve(path, cmd->argv, envp);
    perror(path);
    free_envp(envp);
    free(path);
    _exit(127);
    return (0);
}

void	child_exec(t_shell *shell, t_cmd *cur, int stdin_fd, int pipefd[2])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cur->next)
		run_child_process(shell, cur, stdin_fd, pipefd[1]);
    else
        run_child_process(shell, cur, stdin_fd, STDOUT_FILENO);
}
