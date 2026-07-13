/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 17:39:34 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/13 08:17:19 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int builtin_pwd(t_cmd *cmd)
{
    char    *pwd;
    (void)cmd;

    pwd = getcwd(NULL, 0);
    if (pwd == NULL)
        return (perror("getcwd() failed"), 1);
    ft_putstr_fd(pwd, 1);
    ft_putchar_fd('\n', 1);
    free(pwd);
    return (0);
}
