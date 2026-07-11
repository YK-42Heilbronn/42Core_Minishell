/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:01:03 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/11 16:20:23 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Build t_cmd objects from tokens,
// detect syntax errors around pipes and redirections
// parser output, environment data, and shell state
#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"
# include <stdio.h>
# include <stdlib.h>

typedef enum e_redir_type
{
	R_IN,
	R_OUT,
	R_APPEND,
	R_HEREDOC
}					t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	struct s_redir	*next;
}					t_redir;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_shell
{
	t_env			*env;
	int				last_status;
	char			*line;
	t_token			*tokens;
	t_cmd			*cmds;
	int				stdin_fd;
	int				stdout_fd;
	int				heredoc_fd;
	int				exit_flag;
}					t_shell;

// src/parser/quote_check.c
int					has_unclosed_quotes(const char *line);

// src/parser/syntax_check.c
int					is_redir_token(t_token_type type);
int					print_syntax_error(char *token);
int					syntax_check_tokens(t_token *tokens);

//  src/parser/parser.c
t_cmd				*parse_tokens(t_token *tokens, t_shell *shell);
t_cmd				*parse_pipeline(t_token **tokens, t_shell *shell);

//  src/parser/parser_commands.c
t_cmd				*parse_command(t_token **tokens, t_shell *shell);
t_cmd				*new_cmd(void);
int					cmd_add_arg(t_cmd *cmd, char *word);

// src/parser/parser_redirs.c
int					parse_redirection(t_cmd *cmd, t_token **tokens);
t_redir				*new_redir(t_redir_type type, char *file);
void				cmd_add_redir(t_cmd *cmd, t_redir *redir);

#endif
