/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:17:27 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/10 20:04:29 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

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

void lvl_up(t_shell *shell)
{
    t_env   *node;
    int     level;
    char    *new_value;

    node = find_env_key(shell->env, "SHLVL");
    if (!node)
    {
        node = new_env_node("SHLVL = 1");
        if (!node)
            return ;
        add_env_back(&shell->env, node);
        return ;
    }
    level = ft_atoi(node->value);
    level++;
    new_value = ft_itoa(level);
    if(!new_value)
        return ;
    free(node->value);
    node->value = new_value;
}
