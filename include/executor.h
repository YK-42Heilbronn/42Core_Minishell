/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:29:48 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/13 11:03:54 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//  Apply redirections, setup pipes and run builtins or external commands

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "libft.h"
# include "parser.h"
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_redir	t_redir;

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

// executor/utils.c
int						cmd_count(t_cmd *cmds);

// executor/exec_child.c
int						execute_child(t_shell *shell, t_cmd *cmd, int stdin_fd,
							int stdout_fd);

// executor/exec_pipeline.c
int						execute_pipeline(t_shell *shell, t_cmd *cmds);

// executor/exec.c
int						execute_shell(t_shell *shell);

#endif
