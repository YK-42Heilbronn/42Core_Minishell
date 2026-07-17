/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 02:54:36 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 02:59:06 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

int	cmd_is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		return (0);
	return (S_ISDIR(st.st_mode));
}

char	*find_executable_path(char *cmd, t_shell *shell)
{
	char	*path_env;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = env_get_value(shell->env, "PATH");
	if (!path_env || !*path_env)
		return (NULL);
	return (search_path_dirs(path_env, cmd));
}

void	check_executable(char *path, char *name)
{
	if (cmd_is_directory(path))
	{
		print_exec_error(name, "Is a directory");
		free(path);
		exit(EXIT_NO_PERM);
	}
	if (access(path, X_OK) != 0)
	{
		print_exec_error(name, "Permission denied");
		free(path);
		exit(EXIT_NO_PERM);
	}
}

/* called inside a forked child right after redirections have been
 * applied. Never returns on success (execve replaces the image);
 * on failure it prints a bash-like message and exits with the
 * matching status (127 not found, 126 no permission / is a directory). */

int	run_external(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**envp;

	if (!cmd->argv || !cmd->argv[0])
		exit(0);
	path = find_executable_path(cmd->argv[0], shell);
	if (!path)
	{
		print_exec_error(cmd->argv[0], "command not found");
		exit(EXIT_CMD_NOT_FOUND);
	}
	check_executable(path, cmd->argv[0]);
	envp = env_to_array(shell->env);
	execve(path, cmd->argv, envp);
	print_exec_error(cmd->argv[0], strerror(errno));
	free(path);
	free_envp(envp);
	exit(EXIT_NO_PERM);
}
