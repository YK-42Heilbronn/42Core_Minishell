/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 17:56:16 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 00:18:56 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//handling the non build in functions 

#include "executor.h"

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return ((ft_strlen(cmd) == 2 && ft_strncmp(cmd, "cd", 2) == 0)
		|| (ft_strlen(cmd) == 4 && ft_strncmp(cmd, "echo", 4) == 0)
		|| (ft_strlen(cmd) == 3 && ft_strncmp(cmd, "env", 3) == 0)
		|| (ft_strlen(cmd) == 4 && ft_strncmp(cmd, "exit", 4) == 0)
		|| (ft_strlen(cmd) == 6 && ft_strncmp(cmd, "export", 6) == 0)
		|| (ft_strlen(cmd) == 3 && ft_strncmp(cmd, "pwd", 3) == 0)
		|| (ft_strlen(cmd) == 5 && ft_strncmp(cmd, "unset", 5) == 0));
}

int	exec_builtin(t_shell *shell, t_cmd *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (ft_strlen(cmd->argv[0]) == 2 && ft_strncmp(cmd->argv[0], "cd", 2) == 0)
		return (builtin_cd(shell, cmd));
	if (ft_strlen(cmd->argv[0]) == 4 && ft_strncmp(cmd->argv[0], "echo",
			4) == 0)
		return (builtin_echo(cmd));
	if (ft_strlen(cmd->argv[0]) == 3 && ft_strncmp(cmd->argv[0], "env", 3) == 0)
		return (builtin_env(shell));
	if (ft_strlen(cmd->argv[0]) == 4 && ft_strncmp(cmd->argv[0], "exit",
			4) == 0)
		return (builtin_exit(shell, cmd));
	if (ft_strlen(cmd->argv[0]) == 6 && ft_strncmp(cmd->argv[0], "export",
			6) == 0)
		return (builtin_export(shell, cmd));
	if (ft_strlen(cmd->argv[0]) == 3 && ft_strncmp(cmd->argv[0], "pwd", 3) == 0)
		return (builtin_pwd(cmd));
	if (ft_strlen(cmd->argv[0]) == 5 && ft_strncmp(cmd->argv[0], "unset",
			5) == 0)
		return (builtin_unset(shell, cmd));
	return (1);
}
