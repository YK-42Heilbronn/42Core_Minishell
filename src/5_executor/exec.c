/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 23:45:06 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 03:05:42 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

int	cmd_count(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

void	exec_single_cmd(t_cmd *cmd, t_shell *shell)
{
	if (!cmd->argv || !cmd->argv[0])
		return (run_redir_only(cmd, shell));
	if (is_builtin(cmd->argv[0]))
		return (run_builtin_foreground(cmd, shell));
	run_external_in_child(cmd, shell);
}

/* upfront heredoc collection, then dispatch to the single-command path
 * (may run builtins directly in the shell, no fork) or the pipeline path
 * (always forks, one child per command). */

void	execute_cmds(t_shell *shell)
{
	if (!shell->cmds)
		return ;
	if (handle_heredocs(shell) == -1)
		return ;
	if (cmd_count(shell->cmds) == 1)
		exec_single_cmd(shell->cmds, shell);
	else
		exec_pipeline(shell->cmds, shell);
	reap_leftover_children();
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
