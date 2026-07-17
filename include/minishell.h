/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 16:22:02 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/17 03:12:42 by ileongar         ###   ########.fr       */
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
# include "parser.h"


/* readline & history */
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_token 	t_token;
typedef struct s_cmd 	t_cmd;
typedef struct s_env 	t_env;
typedef struct s_redir 	t_redir;

// main/main.c
void	shell_exit(t_shell *shell, int status);
void	run_one_line(t_shell *shell, char *line);
void	shell_loop(t_shell *shell);
int	    main(int argc, char **argv, char **envp);

// main/signals.c
void	sigint_handler(int sig);
void	set_signals_interactive(void);
void	set_signals_exec_child(void);
void	set_signals_wait_child(void);
void	set_signals_heredoc(void);


//main/utils.c
void	write_str(int fd, const char *s);
void	print_exec_error(char *name, char *msg);
void	free_cmd(t_cmd **cmd);
void	free_cmds(t_cmd *cmds);
void	set_status_from_wait(t_shell *shell, int status);

#endif