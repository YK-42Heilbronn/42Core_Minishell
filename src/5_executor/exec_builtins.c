/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 17:56:16 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 02:35:48 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strncmp(cmd, "echo", 5) || !ft_strncmp(cmd, "cd", 3)
		|| !ft_strncmp(cmd, "pwd", 4) || !ft_strncmp(cmd, "export", 7)
		|| !ft_strncmp(cmd, "unset", 6) || !ft_strncmp(cmd, "env", 4)
		|| !ft_strncmp(cmd, "exit", 5))
		return (1);
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_shell *shell)
{
	char	*name;

	name = cmd->argv[0];
	if (!ft_strncmp(name, "echo", 5))
		return (builtin_echo(cmd->argv));
	if (!ft_strncmp(name, "cd", 3))
		return (builtin_cd(cmd->argv, shell));
	if (!ft_strncmp(name, "pwd", 4))
		return (builtin_pwd());
	if (!ft_strncmp(name, "export", 7))
		return (builtin_export(cmd->argv, shell));
	if (!ft_strncmp(name, "unset", 6))
		return (builtin_unset(cmd->argv, shell));
	if (!ft_strncmp(name, "env", 4))
		return (builtin_env(shell));
	return (builtin_exit(cmd->argv, shell));
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
