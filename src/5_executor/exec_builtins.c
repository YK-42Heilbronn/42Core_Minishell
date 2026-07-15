/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 17:56:16 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 23:32:38 by ileongar         ###   ########.fr       */
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

// int	is_builtin(char *cmd)
// {
// 	if (!cmd)
// 		return (0);
// 	return ((ft_strlen(cmd) == 2 && ft_strncmp(cmd, "cd", 2) == 0)
// 		|| (ft_strlen(cmd) == 4 && ft_strncmp(cmd, "echo", 4) == 0)
// 		|| (ft_strlen(cmd) == 3 && ft_strncmp(cmd, "env", 3) == 0)
// 		|| (ft_strlen(cmd) == 4 && ft_strncmp(cmd, "exit", 4) == 0)
// 		|| (ft_strlen(cmd) == 6 && ft_strncmp(cmd, "export", 6) == 0)
// 		|| (ft_strlen(cmd) == 3 && ft_strncmp(cmd, "pwd", 3) == 0)
// 		|| (ft_strlen(cmd) == 5 && ft_strncmp(cmd, "unset", 5) == 0));
// }

// int	exec_builtin(t_shell *shell, t_cmd *cmd)
// {
// 	if (!cmd || !cmd->argv || !cmd->argv[0])
// 		return (1);
// 	if (ft_strlen(cmd->argv[0]) == 2 && ft_strncmp(cmd->argv[0], "cd", 2) == 0)
// 		return (builtin_cd(shell, cmd));
// 	if (ft_strlen(cmd->argv[0]) == 4 && ft_strncmp(cmd->argv[0], "echo",
// 			4) == 0)
// 		return (builtin_echo(cmd));
// 	if (ft_strlen(cmd->argv[0]) == 3 && ft_strncmp(cmd->argv[0], "env", 3) == 0)
// 		return (builtin_env(shell));
// 	if (ft_strlen(cmd->argv[0]) == 4 && ft_strncmp(cmd->argv[0], "exit",
// 			4) == 0)
// 		return (builtin_exit(shell, cmd));
// 	if (ft_strlen(cmd->argv[0]) == 6 && ft_strncmp(cmd->argv[0], "export",
// 			6) == 0)
// 		return (builtin_export(shell, cmd));
// 	if (ft_strlen(cmd->argv[0]) == 3 && ft_strncmp(cmd->argv[0], "pwd", 3) == 0)
// 		return (builtin_pwd(cmd));
// 	if (ft_strlen(cmd->argv[0]) == 5 && ft_strncmp(cmd->argv[0], "unset",
// 			5) == 0)
// 		return (builtin_unset(shell, cmd));
// 	return (1);
// }
