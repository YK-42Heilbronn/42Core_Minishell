/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:56:07 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/15 23:05:10 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

static int	append_char_at(char **result, const char *word, int *i)
{
    *result = str_append_char(*result, word[*i]);
    if (!*result)
        return (0);
    (*i)++;
    return (1);
}

static int	append_exit_status(char **result, t_shell *shell, int *i)
{
    char	*status;

    status = expand_exit_status(shell);
    if (!status)
        return (*result=NULL, 0);
    *result = str_append_str(*result, status);
    free(status);
    if (!*result)
        return (0);
    *i += 2;
    return (1);
}

void print_env(t_shell *shell)
{
    t_env *next;

    next = shell->env;
    if (!next)
        printf("env:null\n");
    while (next)
    {
        printf("key:%s|value:%s\n", next->key, next->value);
        next = next->next;
    }
}

static int	append_env_var(char **result, const char *word, t_shell *shell, int *i)
{
    char	*name;
    char	*value;
    int		len;

    len = var_name_len(&word[*i + 1]);
    if (len <= 0)
        return (append_char_at(result, word, i));
    name = extract_var_name(&word[*i + 1], len);
    if (!name)
        return (0);
    value = env_get_value(shell->env, name);
    // print_env(shell);
    // printf("name:%s\n", name);
    // printf("value:%s\n", value);
    free(name);
    if (value)
        *result = str_append_str(*result, value);
    else
        *result = str_append_str(*result, "");
    if (!*result)
        return (0);
    *i += len + 1;
    // printf("re::%s\n", *result);
    return (1);
}

static int	handle_dollar(char **result, const char *word, t_shell *shell, int *i)
{
    char	next;

    next = word[*i + 1];
    if (next == '?')
        return (append_exit_status(result, shell, i));
    if (is_valid_var_start(next))
        return (append_env_var(result, word, shell, i));
    return (append_char_at(result, word, i));
}

/*
Behavior::
If sq == 1, return ft_strdup(word) with no expansion.
If not single-quoted:
	copy normal chars as-is
	when seeing $?, replace with expand_exit_status(shell)
	when seeing $ followed by valid var start, extract var name and replace using env_get_value(shell->env, key)
	if variable does not exist, append empty string
	if $ is followed by invalid char or end of string, copy $ literally
dq does not block expansion; it only matters because double quotes allow $ expansion while single quotes do not
*/
// sq = single quoted, dq = double quoted
// all the shell variables(session_custom+user+system) are added to ENV, can be extracted from the env_linked_list
// expansion only starts if there is an $ sign
// expansion rules:
// single quotes: no expansion
// double quotes: expansion takes place
// char	*expand_word(const char *word, t_shell *shell, int sq, int dq)
// {
//     char	*result;
//     int		i;

//     (void)dq;
//     if (!word)
//         return (ft_strdup(""));
//     word = ft_strtrim(word, "\"");
//     if (sq)
//         return (ft_strdup(word));
//     result = malloc(1);
//     if (!result)
//         return (NULL);
//     result[0] = '\0';
//     i = 0;
//     printf("%s-%zu\n", word, ft_strlen(word));
//     while (word[i])
//     {
//         if (word[i] == '$')
//         {
//             if (!handle_dollar(&result, word, shell, &i))
//                 return (free(result), NULL);
//         }
//         else if (!append_char_at(&result, word, &i))
//             return (free(result), NULL);
//         printf("result:%s,%d\n", result, i);
//     }
//     return (result);
// }

char	*expand_word(const char *word, t_shell *shell, int q_state)
{
    char	*result;
    int		i;

    if (!word)
        return (ft_strdup(""));
    result = ft_strdup("");
    if (!result)
        return (NULL);
    i = 0;
    while (word[i])
    {
        if (word[i] == '\'' && q_state == 1)
        {
            i++;
            continue;
        }
        if (word[i] == '"' && q_state == 2)
        {
            i++;
            continue;
        }
        if (word[i] == '$' && (q_state == 2 || q_state == 0))  // && q_state == 2
        {
            if (!handle_dollar(&result, word, shell, &i))
                return (free(result), NULL);
            continue;
        }
        if (!append_char_at(&result, word, &i))
            return (free(result), NULL);
    }
    return (result);
}
