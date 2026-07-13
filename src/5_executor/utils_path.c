/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 23:45:09 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/13 17:29:03 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	free_split(char **arr);

char	*join_path(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, cmd);
	free(tmp);
	return (res);
}

char	*get_path_env(t_env *env)
{
	while (env)
	{
		if (!ft_strncmp(env->key, "PATH", 5) && env->key[4] == '\0')
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*find_cmd_in_path(char **dirs, char *cmd)
{
	char	*full;
	int		i;

	i = 0;
	while (dirs[i])
	{
		full = join_path(dirs[i], cmd);
		if (full && access(full, X_OK) == 0)
			break ;
		free(full);
		i++;
	}
	return (NULL);
}

char	*cmd_path_resolution(t_shell *shell, char *cmd)
{
	char	**dirs;
	char	*path;
	char	*full;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = get_path_env(shell->env);
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	full = find_cmd_in_path(dirs, cmd);
	free_split(dirs);
	return (full);
}

void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
