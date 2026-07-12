/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 19:10:23 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/12 20:25:18 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parser.h"

//TODO: rewrite set_env_value, get rid of ?
//what id chdir
//divide buildin function

t_env   *find_env(t_env *env, const char *key)
{
    while (env)
    {
        if (!ft_strncmp(env->key, key, ft_strlen(key) + 1))
            return (env);
        env = env->next;
    }
    return (NULL);
}

char    *get_env_value(t_env *env, char *key)
{
    t_env   *node;

    node = find_env(env, key);
    if (!node)
        return (NULL);
    return (node->value);
}

int set_env_value(t_env **env, const char *key, const char *value)
{
    t_env   *node;
    
    if (!env || !key)
        return (1);
    node = *env;
    while (node)
    {
        if (!ft_strncmp(node->key, key, ft_strlen(key) + 1))
        {
            free(node->value);
            node->value = value ? ft_strdup(value) : NULL;
            return (0);
        }
        node = node->next;
    }
    node = malloc(sizeof(t_env));
    if (!node)
        return (1);
    node->key = ft_strdup(key);
    node->value = value ? ft_strdup(value) : NULL;
    node->next = *env;
    *env = node;
    return (0);
}

void    cd_error_print(const char *arg)
{
    write(2, "minishell: cd ", 15);
    write(2, arg, ft_strlen(arg));
    write(2, ": ", 2);
    write(2, strerror(errno), ft_strlen(strerror(errno)));
    write(2, "\n", 1);
}

int builtin_cd(t_shell *shell, t_cmd *cmd)
{
    char    *target;
    char    cwd[4096];
    char    *oldpwd;

    if (!shell || !cmd)
        return (1);
    if (!cmd->argv[1])
    {
        target = get_env_value(shell->env, "HOME");
        if (!target)
            return (write(2, "minishell: cd: HOME not set\n", 28), 1);
    }
    else
        target = cmd->argv[1];
    if (!getcwd(!cwd, sizeof(cwd)))
        cwd[0] = '\0';
    oldcwd = cwd[0] ? ft_strdup(cwd) : NULL;
    if (chdir(target) < 0)
    {
        cd_error_print(target);
        if (oldpwd)
            free(oldpwd);
        return (1);
    }
    if (oldpwd)
    {
        set_env_value(&shell->env, "OLDPWD", oldpwd);
        free(oldpwd);
    }
    if(!getcwd(cwd, sizeof(cwd)))
        return (1);
    set_env_value(&shell->env, "PWD", cwd);
    return (0);
}
