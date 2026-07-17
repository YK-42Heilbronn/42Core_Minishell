/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 21:17:30 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/17 03:10:27 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

volatile sig_atomic_t	g_signal = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write_str(1, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/* used while waiting at the prompt: ctrl-C reprints the prompt on a new
 * line, ctrl-\ does nothing, ctrl-D is handled separately in main.c
 * (readline() returning NULL). */

void	set_signals_interactive(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/* used inside a forked child right before execve/builtin: restore default
 * dispositions so the child behaves like any normal foreground process. */

void	set_signals_exec_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/* used by the parent while it is waiting on a foreground child: the shell
 * itself must not die from ctrl-C / ctrl-\, only the child should react. */

void	set_signals_wait_child(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/* used by the heredoc-reader child: ctrl-C should kill it (so the parent
 * can detect the interruption via WIFSIGNALED), ctrl-\ is ignored. */

void	set_signals_heredoc(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}
