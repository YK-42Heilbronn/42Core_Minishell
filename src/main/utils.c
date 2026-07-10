/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 19:40:37 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/10 20:04:23 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_env_list(t_env *env)
{
    t_env   *tmp;
    
    while(env)
    {
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}

t_env    *new_env_node(const char *line)
{
    t_env   *node;
    char    *equal;
    char    *key;
    char    *value;

    node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    equal = ft_strchr(line, '=');
    if (equal)
    {
        key = ft_substr(line, 0, equal - line);
        value = ft_strdup(equal + 1);
    }
    else
    {
        key = ft_strdup(line);
        value = ft_strdup("");
    }
    if (!key || !value)
        return (free(key), free(value), free(node), NULL);
    node->key = key;
    node->value = value;
    node->next = NULL;
    return (node);
}

void add_env_back(t_env **env, t_env *new_node)
{
    t_env   *current;
    
    if (!*env)
    {
        *env = new_node;
        return ;
    }
    current = *env;
    while (current->next)
        current = current->next;
    current->next = new_node;
}

t_env    *find_env_key(t_env env, char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return (env);
        env = env->next;   
    }
    return(NULL);
}
