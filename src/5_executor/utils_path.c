/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 23:45:09 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/15 22:47:45 by ileongar         ###   ########.fr       */
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

char	*build_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full);
}

void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	*find_executable_path(char *cmd, t_shell *shell)
{
	char	*path_env;
	char	**dirs;
	char	*candidate;
	int		i;

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
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		candidate = build_path(dirs[i], cmd);
		if (candidate && access(candidate, F_OK) == 0)
			return (free_split(dirs), candidate);
		free(candidate);
		i++;
	}
	return (free_split(dirs), NULL);
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
	if (cmd_is_directory(path))
	{
		print_exec_error(cmd->argv[0], "Is a directory");
		free(path);
		exit(EXIT_NO_PERM);
	}
	if (access(path, X_OK) != 0)
	{
		print_exec_error(cmd->argv[0], "Permission denied");
		free(path);
		exit(EXIT_NO_PERM);
	}
	envp = env_to_array(shell->env);
	execve(path, cmd->argv, envp);
	print_exec_error(cmd->argv[0], strerror(errno));
	free(path);
	free_envp(envp);
	exit(EXIT_NO_PERM);
}


// char	*join_path(const char *dir, const char *cmd)
// {
// 	char	*tmp;
// 	char	*res;

// 	tmp = ft_strjoin(dir, "/");
// 	if (!tmp)
// 		return (NULL);
// 	res = ft_strjoin(tmp, cmd);
// 	free(tmp);
// 	return (res);
// }

// // char	*get_path_env(t_env *env)
// // {
// // 	while (env)
// // 	{
// // 		if (!ft_strncmp(env->key, "PATH", 5) && env->key[4] == '\0')
// // 			return (env->value);
// // 		env = env->next;
// // 	}
// // 	return (NULL);
// // }

// char    *get_path_env(t_env *env)
// {
//     while (env)
//     {
//         if (env->key
//             && ft_strlen(env->key) == 4
//             && ft_strncmp(env->key, "PATH", 4) == 0)
//             return (env->value);
//         env = env->next;
//     }
//     return (NULL);
// }

// char	*find_cmd_in_path(char **dirs, char *cmd)
// {
// 	char	*full;
// 	int		i;

// 	i = 0;
// 	while (dirs[i])
// 	{
// 		full = join_path(dirs[i], cmd);
// 		if (full && access(full, X_OK) == 0)
// 			return (full);
// 		free(full);
// 		i++;
// 	}
// 	return (NULL);
// }


// char	*cmd_path_resolution(t_shell *shell, char *cmd)
// {
// 	char	**dirs;
// 	char	*path;
// 	char	*full;

// 	if (!cmd || !*cmd)
// 		return (NULL);
// 	if (ft_strchr(cmd, '/'))
// 		return (ft_strdup(cmd));
// 	path = get_path_env(shell->env);
// 	if (!path)
// 		return (NULL);
// 	dirs = ft_split(path, ':');
// 	if (!dirs)
// 		return (NULL);
// 	full = find_cmd_in_path(dirs, cmd);
// 	free_split(dirs);
// 	return (full);
// }

// void	free_split(char **arr)
// {
// 	int	i;

// 	if (!arr)
// 		return ;
// 	i = 0;
// 	while (arr[i])
// 	{
// 		free(arr[i]);
// 		i++;
// 	}
// 	free(arr);
// }
