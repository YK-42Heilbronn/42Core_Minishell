/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:21:38 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 22:47:20 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

void	exec_pipeline(t_cmd *cmds, t_shell *shell)
{
	int		n;
	int		*pipes;
	pid_t	*pids;
	int		status;

	n = cmd_count(cmds);
	pipes = create_pipes(n);
	pids = malloc(sizeof(pid_t) * n);
	if ((n > 1 && !pipes) || !pids)
	{
		free(pipes);
		free(pids);
		shell->last_status = 1;
		return ;
	}
	fork_pipeline(cmds, shell, pipes, pids, n);
	close_all_pipes(pipes, n);
	free(pipes);
	set_signals_wait_child();
	wait_pipeline(pids, n, &status);
	set_signals_interactive();
	set_status_from_wait(shell, status);
	free(pids);
}

int	*create_pipes(int n)
{
	int	*pipes;
	int	i;

	if (n <= 1)
		return (NULL);
	pipes = malloc(sizeof(int) * 2 * (n - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < n - 1)
	{
		if (pipe(&pipes[i * 2]) == -1)
			return (free(pipes), NULL);
		i++;
	}
	return (pipes);
}

void	close_all_pipes(int *pipes, int n)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < 2 * (n - 1))
	{
		close(pipes[i]);
		i++;
	}
}

/* wires stdin/stdout from the shared pipe array, closes every pipe fd
 * (not just the two in use), then applies redirections (which may
 * override the piped stdin/stdout) before running builtin or external. */
void	run_pipeline_child(t_cmd *cmd, t_shell *shell, int *pipes,
		int idx, int n)
{
	set_signals_exec_child();
	if (idx > 0)
		dup2(pipes[(idx - 1) * 2], STDIN_FILENO);
	if (idx < n - 1)
		dup2(pipes[idx * 2 + 1], STDOUT_FILENO);
	close_all_pipes(pipes, n);
	if (apply_redirections(cmd, shell) == -1)
		exit(1);
	if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
		exit(exec_builtin(cmd, shell));
	run_external(cmd, shell);
}

void	fork_pipeline(t_cmd *cmds, t_shell *shell, int *pipes, pid_t *pids,
		int n)
{
	t_cmd	*cur;
	int		idx;

	cur = cmds;
	idx = 0;
	while (cur)
	{
		pids[idx] = fork();
		if (pids[idx] == -1)
			pids[idx] = 0;
		else if (pids[idx] == 0)
			run_pipeline_child(cur, shell, pipes, idx, n);
		cur = cur->next;
		idx++;
	}
}

void	wait_pipeline(pid_t *pids, int n, int *status)
{
	int	i;
	int	st;

	i = 0;
	st = 0;
	while (i < n)
	{
		if (pids[i] > 0)
			waitpid(pids[i], &st, 0);
		if (i == n - 1)
			*status = st;
		i++;
	}
}


// int	wait_pipeline(pid_t last_pid, t_shell *shell)
// {
// 	int		status;
// 	int		last_status;
// 	pid_t	pid;

// 	last_status = 0;
// 	if (last_pid <= 0)
// 		return (shell->last_status);
// 	status = 0;
// 	pid = waitpid(-1, &status, 0);
// 	while (pid > 0)
// 	{
// 		if (pid == last_pid)
// 			last_status = status;
// 		pid = waitpid(-1, &status, 0);
// 	}
// 	if (WIFEXITED(last_status))
// 		shell->last_status = WEXITSTATUS(last_status);
// 	else if (WIFSIGNALED(last_status))
// 		shell->last_status = 128 + WTERMSIG(last_status);
// 	else
// 		shell->last_status = 1;
// 	return (shell->last_status);
// }

// static void	parent_exec(t_cmd *cur, t_pipe *ctx, int pipefd[2])
// {
// 	if (ctx->stdin_fd != STDIN_FILENO)
// 		close(ctx->stdin_fd);
// 	if (cur->next)
// 	{
// 		close(pipefd[1]);
// 		ctx->stdin_fd = pipefd[0];
// 	}
// 	else
// 	{
// 		close(pipefd[0]);
// 		close(pipefd[1]);
// 	}
// }

// static int	run_pipeline_step(t_shell *shell, t_cmd *cur, t_pipe *ctx)
// {
// 	int		pipefd[2];
// 	pid_t	pid;

// 	if (cur->next && pipe(pipefd) < 0)
// 		return (perror("pipe"), 1);
// 	pid = fork();
// 	if (pid < 0)
// 		return (perror("fork"), 1);
// 	if (pid == 0)
// 		child_exec(shell, cur, ctx->stdin_fd, pipefd);
// 	if (ctx->last_pid == -1)
// 		ctx->last_pid = pid;
// 	parent_exec(cur, ctx, pipefd);
// 	return (0);
// }

// int	execute_pipeline(t_shell *shell, t_cmd *cmds)
// {
// 	t_pipe	ctx;
// 	t_cmd	*cur;

// 	if (!shell || !cmds)
// 		return (1);
// 	if (cmd_count(cmds) == 1 && cmds->argv && cmds->argv[0]
// 		&& is_builtin(cmds->argv[0]))
// 		return (exec_builtin(shell, cmds));
// 	cur = cmds;
// 	ctx.stdin_fd = STDIN_FILENO;
// 	ctx.last_pid = -1;
// 	while (cur)
// 	{
// 		if (run_pipeline_step(shell, cur, &ctx) != 0)
// 			return (1);
// 		cur = cur->next;
// 	}
// 	return (wait_pipeline(ctx.last_pid, shell));
// }
