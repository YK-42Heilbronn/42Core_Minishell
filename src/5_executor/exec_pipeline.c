/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:21:38 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 02:46:27 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

void	exec_pipeline(t_cmd *cmds, t_shell *shell)
{
	t_pipe_ctx	ctx;
	int			status;

	ctx.n = cmd_count(cmds);
	ctx.pipes = create_pipes(ctx.n);
	ctx.pids = malloc(sizeof(pid_t) * ctx.n);
	if ((ctx.n > 1 && !ctx.pipes) || !ctx.pids)
	{
		free(ctx.pipes);
		free(ctx.pids);
		shell->last_status = 1;
		return ;
	}
	fork_pipeline(cmds, shell, &ctx);
	close_all_pipes(ctx.pipes, ctx.n);
	free(ctx.pipes);
	set_signals_wait_child();
	wait_pipeline(ctx.pids, ctx.n, &status);
	set_signals_interactive();
	set_status_from_wait(shell, status);
	free(ctx.pids);
}
