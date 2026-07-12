/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 17:56:16 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/12 20:27:52 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"

int is_builtin(const char *cmd)
{
    return (cmd && (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "pwd")
     || !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "cd")
     || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
     || !ft_strcmp(cmd, "exit")))
}

int exec_builtin(t_shell *shell, t_cmd *cmd)
{
    if (!cmd || !cmd->argv || !cmd->argv[0])
        return (1);
    if (!ft_strcmp(cmd->argv[0], "echo"))
        return (builtin_echo(cmd));
    if (!ft_strcmp(cmd->argv[0], "pwd"))
        return (builtin_pwd(cmd));
    if (!ft_strcmp(cmd->argv[0], "env"))
        return (builtin_env(cmd));
    if (!ft_strcmp(cmd->argv[0], "cd"))
        return (builtin_cd(shell, cmd));
    if (!ft_strcmp(cmd->argv[0], "export"))
        return (builtin_export(shell, cmd));
    if (!ft_strcmp(cmd->argv[0], "unset"))
        return (builtin_unset(shell, cmd));
    if (!ft_strcmp(cmd->argv[0], "exit"))
        return (builtin_exit(shell, cmd));
    return (1);
}

