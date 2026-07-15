/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:29:48 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/15 21:45:27 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//  Apply redirections, setup pipes and run builtins or external commands

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include "libft.h"

# include "parser.h"
# include "minishell.h"

typedef struct s_redir	t_redir;
typedef struct s_shell t_shell;
typedef struct s_cmd t_cmd;
typedef struct s_env t_env;

typedef struct s_pipe
{
	int					stdin_fd;
	int					pipefd[2];
	pid_t				last_pid;
}						t_pipe;

// // @Comment: Linkedlist
// typedef struct s_cmd
// {
//     char            **argv;
//     t_redir         *redirs;
//     int             pipe_in;
//     int             pipe_out;
//     struct s_cmd    *next;
// }   t_cmd;

// builtins/cd.c
int						builtin_cd(t_shell *shell, t_cmd *cmd);

// builtins/echo.c
int						builtin_echo(t_cmd *cmd);

// builtins/env.c
int						builtin_env(t_shell *shell);

// builtins/exit.c
int						builtin_exit(t_shell *shell, t_cmd *cmd);

// builtins/export.c
int						builtin_export(t_shell *shell, t_cmd *cmd);

// builtins/pwd.c
int						builtin_pwd(t_cmd *cmd);

// builtins/unset.c
int						builtin_unset(t_shell *shell, t_cmd *cmd);

// executor/exec_builtins.c
int						is_builtin(const char *cmd);
int						exec_builtin(t_shell *shell, t_cmd *cmd);

// executor/utils_path.c
char					*cmd_path_resolution(t_shell *shell, char *cmd);
char					*get_path_env(t_env *env);
char					*find_cmd_in_path(char **dirs, char *cmd);
void					free_split(char **arr);

// executor/exec_child.c
int						run_child_process(t_shell *shell, t_cmd *cmd,
							int stdin_fd, int stdout_fd);
void					child_exec(t_shell *shell, t_cmd *cur, int stdin_fd,
							int pipefd[2]);

// executor/exec_pipeline.c
int						execute_pipeline(t_shell *shell, t_cmd *cmds);

// executor/exec.c
int						execute_shell(t_shell *shell);

// executor/redirections.c
int						open_redir_fd(t_shell *shell, t_redir *r);
int						apply_input_redir(t_redir *r, int fd);
int						apply_output_redir(int fd);
int						apply_one_redir(t_shell *shell, t_redir *r);
int						apply_redirs(t_shell *shell, t_cmd *cmd, int is_child);

#endif
