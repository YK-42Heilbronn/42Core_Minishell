/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 23:45:09 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 02:58:53 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

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

char	*search_path_dirs(char *path_env, char *cmd)
{
	char	**dirs;
	char	*candidate;
	int		i;

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
