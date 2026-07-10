/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 16:22:02 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/10 20:04:08 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"

/* main */
int     main(int argc, char **argv, char **envp);

/* init */
void    init_shell(t_shell *sh, char **envp);
t_env   *init_env(char **envp);
void    lvl_up(t_shell *shell);

/* utils */
void    free_env_list(t_env *env);
t_env   *new_env_node(const char *line);
void    add_env_back(t_env **env, t_env *new_node);
t_env   *find_env_key(t_env env, char *key);

#endif