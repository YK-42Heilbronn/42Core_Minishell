/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 23:45:02 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 22:47:12 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

/* a command that is only redirections, e.g. `> file` with no argv[0]:
 * apply the redirection side effects, run nothing, restore fds. */
void	run_redir_only(t_cmd *cmd, t_shell *shell)
{
	int	saved_in;
	int	saved_out;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (apply_redirections(cmd, shell) == -1)
		shell->last_status = 1;
	else
		shell->last_status = 0;
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
}

/* builtins that must affect the real shell (cd, export, unset, exit...)
 * run here, in the parent, without forking. Redirections are applied and
 * then reverted around the call so `pwd > file` etc. behave correctly. */
void	run_builtin_foreground(t_cmd *cmd, t_shell *shell)
{
	int	saved_in;
	int	saved_out;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (apply_redirections(cmd, shell) == -1)
		shell->last_status = 1;
	else
		shell->last_status = exec_builtin(cmd, shell);
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
}

void	run_external_in_child(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		shell->last_status = 1;
		return ;
	}
	if (pid == 0)
	{
		set_signals_exec_child();
		if (apply_redirections(cmd, shell) == -1)
			exit(1);
		run_external(cmd, shell);
	}
	set_signals_wait_child();
	waitpid(pid, &status, 0);
	set_signals_interactive();
	set_status_from_wait(shell, status);
}

void	exec_single_cmd(t_cmd *cmd, t_shell *shell)
{
	if (!cmd->argv || !cmd->argv[0])
		return (run_redir_only(cmd, shell));
	if (is_builtin(cmd->argv[0]))
		return (run_builtin_foreground(cmd, shell));
	run_external_in_child(cmd, shell);
}

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

// int run_child_process(t_shell *shell, t_cmd *cmd, int stdin_fd, int stdout_fd)
// {
//     char    *path;
//     char    **envp;

//     if (stdin_fd != STDIN_FILENO)
//     {
//         if (dup2(stdin_fd, STDIN_FILENO) < 0)
//             _exit(1);
//         close(stdin_fd);
//     }
//     if (stdout_fd != STDOUT_FILENO)
//     {
//         if (dup2(stdout_fd, STDOUT_FILENO) < 0)
//             _exit(1);
//         close(stdout_fd);
//     }
//     if (apply_redirs(shell, cmd, 1))
//         _exit(1);
//     if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
//         _exit(exec_builtin(shell, cmd));
//     path = cmd_path_resolution(shell, cmd->argv[0]);
//     if (!path)
//     {
//         perror(cmd->argv[0]);
//         _exit(127);
//     }
//     envp = env_to_array(shell->env);
//     if (!envp)
//     {
//         free(path);
//         _exit(1);
//     }
//     execve(path, cmd->argv, envp);
//     perror(path);
//     free_envp(envp);
//     free(path);
//     _exit(127);
//     return (0);
// }

// void	child_exec(t_shell *shell, t_cmd *cur, int stdin_fd, int pipefd[2])
// {
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// 	if (cur->next)
// 		run_child_process(shell, cur, stdin_fd, pipefd[1]);
//     else
//         run_child_process(shell, cur, stdin_fd, STDOUT_FILENO);
// }
