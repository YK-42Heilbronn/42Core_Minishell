/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 00:02:15 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/12 19:17:09 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
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

static void	cmd_add_back(t_cmd **list, t_cmd *new_cmd)
{
	t_cmd	*last;

	if (!list || !new_cmd)
		return ;
	if (!*list)
	{
		*list = new_cmd;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = new_cmd;
}
