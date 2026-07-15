/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:29:48 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/16 00:30:57 by ileongar         ###   ########.fr       */
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
#include <sys/stat.h>

# define EXIT_CMD_NOT_FOUND 127
# define EXIT_NO_PERM 126

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
int	builtin_cd(char **argv, t_shell *shell);

// builtins/echo.c
int	builtin_echo(char **argv);

// builtins/env.c
int	builtin_env(t_shell *shell);

// builtins/exit.c
// int						builtin_exit(t_shell *shell, t_cmd *cmd);
int	builtin_exit(char **argv, t_shell *shell);
int	is_numeric_arg(const char *str);

// builtins/export.c
// int						builtin_export(t_shell *shell, t_cmd *cmd);
int	builtin_export(char **argv, t_shell *shell);
int	is_valid_identifier(const char *str);
void	print_export_list(t_shell *shell);

// // builtins/pwd.c
// int						builtin_pwd(t_cmd *cmd);
int	builtin_pwd(void);

// builtins/unset.c
// int						builtin_unset(t_shell *shell, t_cmd *cmd);
int	builtin_unset(char **argv, t_shell *shell);

// executor/exec_builtins.c
// int						is_builtin(char *cmd);
// int						exec_builtin(t_shell *shell, t_cmd *cmd);
int	is_builtin(char *cmd);
int	exec_builtin(t_cmd *cmd, t_shell *shell);

// executor/utils_path.c
// char					*cmd_path_resolution(t_shell *shell, char *cmd);
// char					*get_path_env(t_env *env);
// char					*find_cmd_in_path(char **dirs, char *cmd);
// void					free_split(char **arr);
int	cmd_is_directory(char *path);
char	*build_path(char *dir, char *cmd);
void	free_split(char **arr);
char	*find_executable_path(char *cmd, t_shell *shell);
int	run_external(t_cmd *cmd, t_shell *shell);

// executor/exec_child.c
// int						run_child_process(t_shell *shell, t_cmd *cmd,
// 							int stdin_fd, int stdout_fd);
// void					child_exec(t_shell *shell, t_cmd *cur, int stdin_fd,
// 							int pipefd[2]);
void	exec_single_cmd(t_cmd *cmd, t_shell *shell);
void	run_redir_only(t_cmd *cmd, t_shell *shell);
void	run_builtin_foreground(t_cmd *cmd, t_shell *shell);
void	run_external_in_child(t_cmd *cmd, t_shell *shell);

// executor/exec_pipeline.c
// int						execute_pipeline(t_shell *shell, t_cmd *cmds);
void	exec_pipeline(t_cmd *cmds, t_shell *shell);
int		*create_pipes(int n);
void	close_all_pipes(int *pipes, int n);
void	fork_pipeline(t_cmd *cmds, t_shell *shell, int *pipes, pid_t *pids,
		int n);
void	wait_pipeline(pid_t *pids, int n, int *status);
void	run_pipeline_child(t_cmd *cmd, t_shell *shell, int *pipes,
		int idx, int n);

// executor/exec.c
// int						execute_shell(t_shell *shell);
int	cmd_count(t_cmd *cmds);
void	execute_cmds(t_shell *shell);

// executor/redirections.c
// int						open_redir_fd(t_shell *shell, t_redir *r);
// int						apply_input_redir(t_redir *r, int fd);
// int						apply_output_redir(int fd);
// int						apply_one_redir(t_shell *shell, t_redir *r);
// int						apply_redirs(t_shell *shell, t_cmd *cmd, int is_child);
int	open_input_redir(t_redir *redir);
int	open_output_redir(t_redir *redir, int append);
int	apply_heredoc_redir(t_redir *redir);
int	apply_redirections(t_cmd *cmd, t_shell *shell);

// executor/heredoc.c
int		handle_heredocs(t_shell *shell);
int		read_heredoc(t_redir *redir, t_shell *shell);
int	append_dollar(char **result, const char *line, t_shell *shell,
		int *i);
char	*expand_heredoc_line(const char *line, t_shell *shell);
char	*choose_expand(char *line, t_redir *redir, t_shell *shell);
void	write_heredoc_line(int fd, char *expanded);
int		is_delimiter(char *line, char *delim);
void	heredoc_writer(int *pipefd, t_redir *redir, t_shell *shell);

#endif
