/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 17:39:34 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/12 21:59:00 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/executor.h"

int builtin_pwd(void)
{
    char    *pwd;

    pwd = getcwd(NULL, 0);
    if (pwd == NULL)
        return (perror("getcwd() failed"), 1);
    ft_putstr_fd(pwd, 1);
    ft_putchar_fd('\n', 1);
    free(pwd);
    return (0);
}
