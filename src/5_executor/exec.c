/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 23:45:06 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/13 17:58:17 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

int	execute_shell(t_shell *shell)
{
	int	n;

	if (!shell || !shell->cmds)
		return (1);
	n = cmd_count(shell->cmds);
	if (n == 1 && shell->cmds->argv && shell->cmds->argv[0]
		&& is_builtin(shell->cmds->argv[0]))
		return (exec_builtin(shell, shell->cmds));
	return (execute_pipeline(shell, shell->cmds));
}
