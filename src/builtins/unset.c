/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 21:57:55 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/12 23:08:54 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parser.h"

int is_valid_identifier(const char *s)
{
    int i;

    if (!s || !*s)
        return (0);
    if (!(ft_isalpha(s[0]) || s[0] == '_'))
        return (0);
    i = 1;
    while (s[i])
    {
        if (!(ft_isalnum(s[i]) || s[i] == '_'))
            return (0);
        i++;
    }
    return (1);
}

void unset_one(t_env **env, const char *key)
{
    t_env   *cur;
    t_env   *prev;
    
    if (!env || !*env || !key)
        return ;
    cur = *env;
    prev = NULL;
    while (cur)
    {
        if (!ft_strncmp(cur->key, key, ft_strlen(key) + 1))
        {
            if (prev)
                prev->next = cur->next;
            else
                *env = cur->next;
            free(cur->key);
            if (cur->value)
                free(cur->value);
            free(cur);
            return ;
        }
        prev = cur;
        cur = cur->next;
    }
}

// int builtin_unset(t_shell *shell, t_cmd *cmd)
// {
    
// }