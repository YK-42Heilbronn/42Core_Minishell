/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 17:56:16 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/14 20:38:13 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			3) == 0)
		return (builtin_unset(shell, cmd));
	return (1);
}

// int	exec_builtin(t_shell *shell, t_cmd *cmd)
// {
// 	if (!cmd || !cmd->argv || !cmd->argv[0])
// 		return (1);
// 	if (!ft_strncmp(cmd->argv[0], "echo", ft_strlen("echo")))
// 		return (builtin_echo(cmd));
// 	if (!ft_strncmp(cmd->argv[0], "pwd", ft_strlen("pwd")))
// 		return (builtin_pwd(cmd));
// 	if (!ft_strncmp(cmd->argv[0], "env", ft_strlen("env")))
// 		return (builtin_env(shell));
// 	if (!ft_strncmp(cmd->argv[0], "cd", ft_strlen("cd")))
// 		return (builtin_cd(shell, cmd));
// 	if (!ft_strncmp(cmd->argv[0], "export", ft_strlen("export")))
// 		return (builtin_export(shell, cmd));
// 	if (!ft_strncmp(cmd->argv[0], "unset", ft_strlen("unset")))
// 		return (builtin_unset(shell, cmd));
// 	if (!ft_strncmp(cmd->argv[0], "exit", ft_strlen("exit")))
// 		return (builtin_exit(shell, cmd));
// 	return (1);
// }
