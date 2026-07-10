/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 00:02:15 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/11 00:23:14 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

char    *get_env_value(t_env *env, char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return (env->value);
        env = env->next;   
    }
    return(NULL);
}

