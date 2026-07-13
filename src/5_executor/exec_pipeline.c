/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:21:38 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/13 17:02:35 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	wait_pipeline(pid_t last_pid, t_shell *shell)
{
	int		status;
	int		last_status;
	pid_t	pid;

	last_status = 0;
	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
			last_status = status;
		pid = wait(&status);
	}
	if (WIFEXITED(last_status))
		shell->last_status = WEXITSTATUS(last_status);
	else if (WIFSIGNALED(last_status))
		shell->last_status = 128 + WTERMSIG(last_status);
	else
		shell->last_status = 1;
	return (shell->last_status);
}

static void	parent_exec(t_cmd *cur, t_pipe *ctx, pid_t pid, int pipefd[2])
{
	ctx->last_pid = pid;

	if (ctx->stdin_fd != STDIN_FILENO)
		close(ctx->stdin_fd);
	if (cur->next)
	{
		close(ctx->pipefd[1]);
		ctx->stdin_fd = ctx->pipefd[0];
	}
}

static int	run_pipeline_step(t_shell *shell, t_cmd *cur, int stdin_fd,
		pid_t *last_pid)
{
	int		pipefd[2];
	pid_t	pid;

	if (cur->next && pipe(ctx->pipefd) < 0)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		child_exec(shell, cur, ctx);
	parent_exec(cur, ctx, pid);
	return (0);
}

int	execute_pipeline(t_shell *shell, t_cmd *cmds)
{
	t_pipe	ctx;

	if (!shell || !cmds)
		return (1);
	if (cmd_count(cmds) == 1 && cmds->argv && cmds->argv[0]
		&& is_builtin(cmds->argv[0]))
		return (exec_builtin(shell, cmds));
	cur = cmds;
	ctx.stdin_fd = STDIN_FILENO;
	ctx.last_pid = -1;
	while (cur)
	{
		if (run_pipeline_step(shell, cur, &stdin_fd, &last_pid))
			return (0);
		cur = cur->next;
	}
	return (wait_pipeline(last_pid, shell));
}
