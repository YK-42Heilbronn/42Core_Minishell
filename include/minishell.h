/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 16:22:02 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/13 14:10:56 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "executor.h"
# include "libft.h"
# include "parser.h"
# include <errno.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

/* readline & history */
# include <readline/history.h>
# include <readline/readline.h>

// main/init.c
void	init_shell(t_shell *shell, char **envp);
t_env	*dup_env(char **envp);
void	lvl_up(t_shell *shell);

// main/utils.c

// main/signals.c
void	sigint_handler(int sig);
void	set_sigint(void);
void	setup_signals(void);

// main/process_line.c
int		process_line(t_shell *shell);

// main/cleanup.c
void	free_redirs(t_redir *redirs);
void	free_cmds(t_cmd *cmd);
void	cleanup_command(t_shell *shell);

#endif