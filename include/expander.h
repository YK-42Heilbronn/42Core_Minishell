/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:29:32 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/15 20:25:30 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Resolve environment variables and quote sensitive text
#ifndef EXPAND_H
# define EXPAND_H
# include "libft.h"
# include "parser.h"
// # include "minishell.h"

typedef struct s_env t_env;

/* src/env/env_init.c */
t_env	*env_new_node(const char *key, const char *value);
void	env_add_back(t_env **env, t_env *new_node);
int		env_init(t_shell *shell, char **envp);
t_env	*split_envp_value(char *env_value);
void	free_env(t_env **sh_env);

/* src/env/env_utils.c */
t_env	*env_get_node(t_env *env, const char *key);
char	*env_get_value(t_env *env, const char *key);
int		env_set_value(t_env **env, const char *key, const char *value);
int		env_unset_value(t_env **env, const char *key);

/* src/env/env_to_array.c */
void	free_envp(char **envp);
char	**env_to_array(t_env *env);

// src/expand/expand_utils.c
int		is_valid_var_start(char c);
int		is_valid_var_char(char c);
int		var_name_len(const char *s);
char	*extract_var_name(const char *s, int len);
char	*str_append_char(char *src, char c);
char	*str_append_str(char *src, const char *suffix);

// src/expand/expand_status.c
char	*expand_exit_status(t_shell *shell);

// src/expand/expand_word.c
void	print_env(t_shell *shell);
char	*expand_word(const char *word, t_shell *shell, int q_state);

// src/expand/expand.c
int		expand_one_token(t_token *token, t_shell *shell);
int		expand_tokens(t_token *tokens, t_shell *shell);

#endif
