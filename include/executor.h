/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:29:48 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/12 19:14:24 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//  Apply redirections, setup pipes and run builtins or external commands

#ifndef EXECUTOR_H
# define EXECUTOR_H

#include "parser.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef struct s_redir  t_redir;

// @Comment: Linkedlist
typedef struct s_cmd
{
    char            **argv;
    t_redir         *redirs;
    int             pipe_in;
    int             pipe_out;
    struct s_cmd    *next;
}   t_cmd;

#endif
