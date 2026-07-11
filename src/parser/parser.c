/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 14:51:18 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/11 12:55:53 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer.h"

static void	cmd_add_back(t_cmd **list, t_cmd *new_cmd)
{
	t_cmd	*last;

	if (!list || !new_cmd)
		return ;
	if (!*list)
	{
		*list = new_cmd;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = new_cmd;
}

t_cmd	*parse_pipeline(t_token **tokens, t_shell *shell)
{
	t_cmd	*cmd;

	if (!tokens || !*tokens)
		return (NULL);
	cmd = parse_command(tokens, shell);
	if (!cmd)
		return (NULL);
	if (*tokens && (*tokens)->type == TOK_PIPE)
		*tokens = (*tokens)->next;
	return (cmd);
}

// cur is modified in the parse_pipeline
t_cmd	*parse_tokens(t_token *tokens, t_shell *shell)
{
	t_token	*cur;
	t_cmd	*cmd_list;
	t_cmd	*new_cmd;

	if (!shell)
		return (NULL);
	if (!tokens)
		return (NULL);
	cur = tokens;
	cmd_list = NULL;
	while (cur)
	{
		new_cmd = parse_pipeline(&cur, shell);
		if (!new_cmd)
			return (NULL);
		cmd_add_back(&cmd_list, new_cmd);
	}
	shell->cmds = cmd_list;
	return (cmd_list);
}

// int	process_input(char *line, t_shell *shell)
// {
// 	t_token	*tokens;
// 	t_cmd	*cmds;

// 	if (has_unclosed_quotes(line))
// 		return (print_syntax_error("unclosed quotes"), 1);
// 	tokens = lex_input(line, shell);
// 	if (!tokens)
// 		return (1);
// 	if (syntax_check_tokens(tokens))
// 		return (free_tokens(tokens), 1);
// 	cmds = parse_tokens(tokens, shell);
// 	free_tokens(tokens);
// 	if (!cmds)
// 		return (1);
// 	shell->cmds = cmds;
// 	return (0);
// }
