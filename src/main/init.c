/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:17:27 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/09 00:03:11 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

//TODO: add functions in Header file
//TODO: split into different files

void    init_shell(t_shell *shell, char **envp)
{
    shell->last_status = 0;
    shell->line = NULL;
    shell->tokens = NULL;
    shell->cmds = NULL;
    shell->env = init_env(envp);
    if(!shell->env)
        exit(1);
    lvl_up(shell);
}

t_env   *init_env(char **envp)
{
    t_env   *env;
    t_env   *node;
    int     i;

    env = NULL;
    if (!envp || !envp[0])
        return(NULL);
    i = 0;
    while(envp[i])
    {
        node = new_env_node(envp[i]);
        if (!node)
        {
            free_env_list(env);
            return (NULL);
        }
        add_env_back(&env, node);
        i++;
    }
    return(env);
}

static void free_env_list(t_env *env)
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

static t_env    *new_env_node(const char *line)
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

static void add_env_back(t_env **env, t_env *new_node)
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

static t_env    *find_env_key(t_env env, char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return (env);
        env = env->next;   
    }
    return(NULL);
}

static void lvl_up(t_shell *shell)
{
    t_env   *node;
    int     level;
    char    *new_value;

    node = find_env_key(shell->env, "SHLVL");
    if (!node)
        return ;
    level = ft_atoi(node->value);
    new_value = ft_itoa(level + 1);
    if (!new_value)
        return ;
    free(node->value);
    node->value = new_value;
}
