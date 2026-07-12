/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 23:45:09 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/13 00:25:11 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

//TODO: 
// devide path resolution

char    *join_path(const char *dir, const char *cmd)
{
    char *tmp;
    char *res;

    tmp = ft_strjoin(dir, "/");
    if (!tmp)
        return (NULL);
    res = ft_strjoin(tmp, cmd);
    free (tmp);
    return (res);
}

char *cmd_path_resolution(t_shell *shell, char *cmd)
{
    t_env   *env;
    char    **dirs;
    char    *path;
    char    *full;
    int     i;

    if (!cmd || !*cmd)
        return (NULL);
    if (ft_strchr(cmd, '/'))
        return (ft_strdup(cmd));
    env = shell->env;
    path = NULL;
    while (env)
    {
        if (ft_strncmp(env->key, "PATH", 5) && env->key[4] == '\0')
            path = env->value;
        env = env->next;
    }
    if (!path)
        return (NULL);
    dirs = ft_split(path, ':');
    if (!dirs)
        return (NULL);
    i = 0;
    full = NULL;
    while (dirs[i])
    {
        full = join_path(dirs[i], cmd);
        if (full && access(full, X_OK) == 0)
            break ;
        free(full);
        full = NULL;
        i++;
    }
    free_split(dirs);
    return (full);
}

void    free_split(char **arr)
{
    int i;

    if (!arr)
        return;
    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}
