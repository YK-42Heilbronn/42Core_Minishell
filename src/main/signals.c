/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:17:30 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/10 21:48:13 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void sigint_handler(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void    set_sigint(void)
{
    struct sigaction    sa;

    ft_memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa->sa_mask);
    sa->sa_flags = 0;
    sa->sa_handler = sigint_handler;
    sigaction(SIGINT, &sa, NULL);
}

void    setup_signals(void)
{
    set_sigint();
    signal(SIGQUIT, SIG_IGN);
}
