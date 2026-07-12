/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 17:30:59 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/12 20:25:13 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"

int builtin_echo(t_cmd *cmd)
{
    int i;
    int new_line;

    i = 1;
    new_line = 1;
    if (cmd->argv[1] && ft_strncmp(cmd->argv[1], "-n", 3) == 0)
    {
        new_line = 0;
        i++;
    }
    while (cmd->argv[i])
    {
        write(1, cmd->argv[i], ft_strlen(cmd->argv[i]));
        if (cmd->argv[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (new_line)
        write(1, "\n", 1);
    return (0);
}
