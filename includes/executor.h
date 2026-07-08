/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:29:48 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/08 13:03:28 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//  Apply redirections, setup pipes and run builtins or external commands

#ifndef EXECUTOR_H
# define EXECUTOR_H

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
