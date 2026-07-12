/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 23:35:15 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/13 00:48:27 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// exit with no args: print exit, set shell->exit_flag, exit with shell->last_status
// exit N: print exit, set exit status to N if numeric, else print error and exit with status 2. This is what the minishell subject expects for exit without options

int is_numeric(const char *s)
{
    int i;

    if (!s || !*s)
        return (0);
    i = 0;
    if (s[i] == "+" || s[i] == '-')
        i++;
    while  (s[i])
    {
        if(!ft_isdigit(s[i]))
            return (0);
        i++;
    }
    return (1);
}

long    ft_long(const char *s)
{
    long    res;
    int     sign;
    int     i;

    sign = 1;
    i = 0;
    if (s[i] == '+' || s[i] == '-')
    {
        if(s[i] == '-')
            sign = -1;
        i++;
    }
    res = 0;
    while (s[i] && ft_isdigit(s[i]))
    {
        res = res * 10 + (s[i] - '0');
        i++;
    }
    return (res * sign);
}

int builtin_exit(t_shell *shell, t_cmd *cmd)
{
    long    code;
    
    write(1, "exit\n", 5);
    if(!cmd->argv[1])
    {
        shell->exit_flag = 1;
        return (shell->last_status);
    }
    if (!is_numeric(cmd->argv[1]))
    {
        write(2, "minishell: exit: ", 17);
        write(2, cmd->argv[1], ft_strlen(cmd->argv[1]));
        write(2, ": numeric argument required\n", 28);
        shell->exit_flag = 1;
        return (2);
    }
    if (cmd->argv[2])
    {
        write(2, "minishell: exit: too many agruments\n", 36);
        return (1);
    }
    code = ft_long(cmd->argv[1]);
    shell->exit_flag = 1;
    return ((int)(code & 0xFF));
}
