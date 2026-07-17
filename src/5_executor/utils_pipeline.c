/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 02:44:17 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 03:00:38 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

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

void	run_pipeline_child(t_cmd *cmd, t_shell *shell, t_pipe_ctx *ctx)
{
	set_signals_exec_child();
	if (ctx->idx > 0)
		dup2(ctx->pipes[(ctx->idx - 1) * 2], STDIN_FILENO);
	if (ctx->idx < ctx->n - 1)
		dup2(ctx->pipes[ctx->idx * 2 + 1], STDOUT_FILENO);
	close_all_pipes(ctx->pipes, ctx->n);
	if (apply_redirections(cmd, shell) == -1)
		exit(1);
	if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
		exit(exec_builtin(cmd, shell));
	run_external(cmd, shell);
}

void	fork_pipeline(t_cmd *cmds, t_shell *shell, t_pipe_ctx *ctx)
{
	t_cmd	*cur;

	cur = cmds;
	ctx->idx = 0;
	while (cur)
	{
		ctx->pids[ctx->idx] = fork();
		if (ctx->pids[ctx->idx] == -1)
			ctx->pids[ctx->idx] = 0;
		else if (ctx->pids[ctx->idx] == 0)
			run_pipeline_child(cur, shell, ctx);
		cur = cur->next;
		ctx->idx++;
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
