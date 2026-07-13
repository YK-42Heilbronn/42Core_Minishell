/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 17:56:16 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/13 17:00:28 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	is_builtin(const char *cmd)
{
	return (cmd && (!ft_strncmp(cmd, "echo", ft_strlen("echo"))
			|| !ft_strncmp(cmd, "pwd", ft_strlen("pwd")) || !ft_strncmp(cmd,
				"env", ft_strlen("env")) || !ft_strncmp(cmd, "cd",
				ft_strlen("cd")) || !ft_strncmp(cmd, "export",
				ft_strlen("export")) || !ft_strncmp(cmd, "unset",
				ft_strlen("unset")) || !ft_strncmp(cmd, "exit",
				ft_strlen("exit"))));
}

int	exec_builtin(t_shell *shell, t_cmd *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (!ft_strncmp(cmd->argv[0], "echo", ft_strlen("echo")))
		return (builtin_echo(cmd));
	if (!ft_strncmp(cmd->argv[0], "pwd", ft_strlen("pwd")))
		return (builtin_pwd(cmd));
	if (!ft_strncmp(cmd->argv[0], "env", ft_strlen("env")))
		return (builtin_env(shell));
	if (!ft_strncmp(cmd->argv[0], "cd", ft_strlen("cd")))
		return (builtin_cd(shell, cmd));
	if (!ft_strncmp(cmd->argv[0], "export", ft_strlen("export")))
		return (builtin_export(shell, cmd));
	if (!ft_strncmp(cmd->argv[0], "unset", ft_strlen("unset")))
		return (builtin_unset(shell, cmd));
	if (!ft_strncmp(cmd->argv[0], "exit", ft_strlen("exit")))
		return (builtin_exit(shell, cmd));
	return (1);
}
