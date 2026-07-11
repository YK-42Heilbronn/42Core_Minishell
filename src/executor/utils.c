/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 17:14:32 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/11 17:29:18 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int cmd_count(t_cmd *cmds)
{
    int n;

    n = 0;
    while(cmds)
    {
        n++;
        cmds = cmds->next;
    }
    return(n);
}
