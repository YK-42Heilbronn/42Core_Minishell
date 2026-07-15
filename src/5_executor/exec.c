/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 23:45:06 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 22:47:26 by ileongar         ###   ########.fr       */
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
	reap_zombie_children();
}


// int	execute_shell(t_shell *shell)
// {
// 	int	n;

// 	if (!shell || !shell->cmds)
// 		return (1);
// 	n = cmd_count(shell->cmds);
// 	if (n == 1 && shell->cmds->argv && shell->cmds->argv[0]
// 		&& is_builtin(shell->cmds->argv[0]))
// 		return (exec_builtin(shell, shell->cmds));
// 	return (execute_pipeline(shell, shell->cmds));
// }
