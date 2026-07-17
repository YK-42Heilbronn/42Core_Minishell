/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:29:48 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/17 03:40:27 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "libft.h"
# include "minishell.h"
# include "parser.h"
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define EXIT_CMD_NOT_FOUND 127
# define EXIT_NO_PERM 126

typedef struct s_redir	t_redir;
typedef struct s_shell	t_shell;
typedef struct s_cmd	t_cmd;
typedef struct s_env	t_env;

typedef struct s_pipe
{
	int					stdin_fd;
	int					pipefd[2];
	pid_t				last_pid;
}						t_pipe;

typedef struct s_pipe_ctx
{
	int					*pipes;
	int					idx;
	int					n;
	pid_t				*pids;
}						t_pipe_ctx;

// executor/exec_builtins.c
int						is_builtin(char *cmd);
int						exec_builtin(t_cmd *cmd, t_shell *shell);
void					run_builtin_foreground(t_cmd *cmd, t_shell *shell);

// executor/exec_path.c
int						cmd_is_directory(char *path);
char					*find_executable_path(char *cmd, t_shell *shell);
void					check_executable(char *path, char *name);
int						run_external(t_cmd *cmd, t_shell *shell);

// executor/exec_pipeline.c
void					exec_pipeline(t_cmd *cmds, t_shell *shell);

// executor/exec.c
int						cmd_count(t_cmd *cmds);
void					exec_single_cmd(t_cmd *cmd, t_shell *shell);
void					execute_cmds(t_shell *shell);
void					run_external_in_child(t_cmd *cmd, t_shell *shell);

// executor/heredoc.c
int						handle_heredocs(t_shell *shell);
int						read_heredoc(t_redir *redir, t_shell *shell);
char					*choose_expand(char *line, t_redir *redir,
							t_shell *shell);
void					heredoc_writer(int *pipefd, t_redir *redir,
							t_shell *shell);
char					*expand_heredoc_line(const char *line, t_shell *shell);

// executor/redirections.c
void					run_redir_only(t_cmd *cmd, t_shell *shell);
int						open_input_redir(t_redir *redir);
int						open_output_redir(t_redir *redir, int append);
int						apply_heredoc_redir(t_redir *redir);
int						apply_redirections(t_cmd *cmd, t_shell *shell);

// executor/utils_heredoc.c
int						is_delimiter(char *line, char *delim);
void					write_heredoc_line(int fd, char *expanded);
void					reap_leftover_children(void);
int						append_dollar_var(char **result, const char *line,
							t_shell *shell, int *i);
int						append_dollar(char **result, const char *line,
							t_shell *shell, int *i);

// executor/utils_path.c
char					*build_path(char *dir, char *cmd);
void					free_split(char **arr);
char					*search_path_dirs(char *path_env, char *cmd);

// executor/utils_pipeline.c
int						*create_pipes(int n);
void					close_all_pipes(int *pipes, int n);
void					run_pipeline_child(t_cmd *cmd, t_shell *shell,
							t_pipe_ctx *ctx);
void					fork_pipeline(t_cmd *cmds, t_shell *shell,
							t_pipe_ctx *ctx);
void					wait_pipeline(pid_t *pids, int n, int *status);

// builtins/cd.c
int						builtin_cd(char **argv, t_shell *shell);

// builtins/echo.c
int						builtin_echo(char **argv);

// builtins/env.c
int						builtin_env(t_shell *shell);

// builtins/exit.c
int						builtin_exit(char **argv, t_shell *shell);
int						is_numeric_arg(const char *str);

// builtins/export.c
int						builtin_export(char **argv, t_shell *shell);
int						is_valid_identifier(const char *str);
void					print_export_list(t_shell *shell);

// // builtins/pwd.c
int						builtin_pwd(void);

// builtins/unset.c
int						builtin_unset(char **argv, t_shell *shell);

#endif
