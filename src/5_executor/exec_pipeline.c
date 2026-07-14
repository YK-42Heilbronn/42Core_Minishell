/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:21:38 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/14 23:20:50 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

int	wait_pipeline(pid_t last_pid, t_shell *shell)
{
	int		status;
	int		last_status;
	pid_t	pid;

	last_status = 0;
	if (last_pid <= 0)
		return (shell->last_status);
	status = 0;
	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == last_pid)
			last_status = status;
		pid = waitpid(-1, &status, 0);
	}
	if (WIFEXITED(last_status))
		shell->last_status = WEXITSTATUS(last_status);
	else if (WIFSIGNALED(last_status))
		shell->last_status = 128 + WTERMSIG(last_status);
	else
		shell->last_status = 1;
	return (shell->last_status);
}

static void	parent_exec(t_cmd *cur, t_pipe *ctx, int pipefd[2])
{
	if (ctx->stdin_fd != STDIN_FILENO)
		close(ctx->stdin_fd);
	if (cur->next)
	{
		close(pipefd[1]);
		ctx->stdin_fd = pipefd[0];
	}
	else
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

static int	run_pipeline_step(t_shell *shell, t_cmd *cur, t_pipe *ctx)
{
	int		pipefd[2];
	pid_t	pid;

	if (cur->next && pipe(pipefd) < 0)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		child_exec(shell, cur, ctx->stdin_fd, pipefd);
	if (ctx->last_pid == -1)
		ctx->last_pid = pid;
	parent_exec(cur, ctx, pipefd);
	return (0);
}

int	execute_pipeline(t_shell *shell, t_cmd *cmds)
{
	t_pipe	ctx;
	t_cmd	*cur;

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
		if (run_pipeline_step(shell, cur, &ctx) != 0)
			return (1);
		cur = cur->next;
	}
	return (wait_pipeline(ctx.last_pid, shell));
}
