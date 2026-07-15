/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 16:22:02 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/15 21:39:53 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include "libft.h"
// # include "lexer.h"
# include "parser.h"
// # include "expander.h"
// # include "executor.h"

/* readline & history */
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_token t_token;
typedef struct s_cmd t_cmd;
typedef struct s_env t_env;
typedef struct s_redir t_redir;

// main/init.c
void	init_shell(t_shell *shell, char **envp);
t_env	*dup_env(char **envp);
void	lvl_up(t_shell *shell);

// main/utils.c
// void	free_env_list(t_env *env);
// t_env	*new_env_node(const char *line);
// void	add_env_back(t_env **env, t_env *new_node);
int		cmd_count(t_cmd *cmds);

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