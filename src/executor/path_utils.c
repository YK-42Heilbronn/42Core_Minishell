/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 23:45:09 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/11 00:28:14 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int env_count(t_env *env)
{
    int counter;
    
    counter = 0;
    while (env)
    {
        counter++;
        env = env->next;
    }
    return(counter)
}

char *join_env_pair(char *key, char *value)
{
    char *tmp;
    char *res;
    
    tmp = ft_strjoin(key, "=");
    if(!tmp)
        return(NULL);
    res = ft_strjoin(tmp, value);
    free(tmp);
    return (res);
}

void free_array(char **arr, int i)
{
    while (i >= 0)
        free(arr[i--]);
    free(arr);
}

char **env_to_array(t_env *env)
{
    char    *arr;
    int     i;

    arr = malloc(sizeof(char *) * env_count(env) + 1);
    if (!arr)
        return (NULL);
    i = 0;
    while (env)
    {
        arr[i] = join_env_pair(env->key, env->value);
        if(!arr[i])
            return (free_array(arr, i - 1), NULL);
        i++;
        env = env->next;
    }
    arr[i] = NULL;
    return (arr);
}
