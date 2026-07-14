/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ileongar <ileongar@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 19:10:23 by ileongar          #+#    #+#             */
/*   Updated: 2026/07/14 23:05:40 by ileongar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "expander.h"

t_env	*find_env(t_env *env, const char *key)
{
	while (env)
	{
		if (env->key && ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*node;

	node = find_env(env, key);
	if (!node)
		return (NULL);
	return (node->value);
}

void	cd_error_print(const char *arg)
{
	write(2, "minishell: cd ", 15);
	write(2, arg, ft_strlen(arg));
	write(2, ": ", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
}

static char *get_cd_target(t_shell *shell, t_cmd *cmd)
{
    char    *target;


    if (!cmd->argv[1])
    {
        target = env_get_value(shell->env, "HOME");
        if (!target)
        {
            write(2, "minishell: cd: HOME not set\n", 28);
            return (NULL);
        }
    }
    else
        target = cmd->argv[1];
    return (target);
}

static char	*save_oldpwd(char *cwd)
{
	char	*oldpwd;

	oldpwd = NULL;
	if (cwd[0])
		oldpwd = ft_strdup(cwd);
	return (oldpwd);
}

static void update_pwd_after_cd(t_shell *shell, char *oldpwd)
{
    char    cwd[4096];


    if (oldpwd)
    {
        env_set_value(&shell->env, "OLDPWD", oldpwd);
        free(oldpwd);
    }
    if (!getcwd(cwd, sizeof(cwd)))
        return ;
    env_set_value(&shell->env, "PWD", cwd);
}

int builtin_cd(t_shell *shell, t_cmd *cmd)
{
    char    *target;
    char    cwd[4096];
    char    *oldpwd;


    if (!shell || !cmd)
        return (1);
    target = get_cd_target(shell, cmd);
    if (!target)
        return (1);
    if (!getcwd(cwd, sizeof(cwd)))
        cwd[0] = '\0';
    oldpwd = save_oldpwd(cwd);
    if (chdir(target) < 0)
    {
        cd_error_print(target);
        if (oldpwd)
            free(oldpwd);
        return (1);
    }
    update_pwd_after_cd(shell, oldpwd);
    return (0);
}
