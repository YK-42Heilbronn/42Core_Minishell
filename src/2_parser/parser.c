/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 14:51:18 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/16 00:24:23 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer.h"
#include "expander.h"
#include "minishell.h"

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

// line
//  -> has_unclosed_quotes
//  -> lex_input
//  -> syntax_check_tokens
//  -> expand_tokens
//  -> parse_tokens
//  -> shell->cmds

// int	process_input(char *line, t_shell *shell)
// {
// 	if (!line || !shell)
// 		return (shell->last_status=1, -1);
// 	if (has_unclosed_quotes(line))
// 		return (print_syntax_error("minishell: unclosed quotes"), shell->last_status=1, -1);
// 	if (shell->tokens)
// 		free_tokens(shell->tokens);
// 	shell->tokens = lex_input(line, shell);
// 	if (shell->tokens == NULL)
// 		return (shell->last_status=1, -1);
// 	if (syntax_check_tokens(shell->tokens))
// 		return (free_tokens(shell->tokens), shell->last_status=1, -1);
// 	if (!expand_tokens(shell->tokens, shell))
// 		return (free_tokens(shell->tokens), shell->last_status=1, -1);
// 	if (shell->cmds)
// 		free_cmds(shell->cmds);
// 	shell->cmds = parse_tokens(shell->tokens, shell);
// 	if (!shell->cmds)
// 		return (shell->last_status=1, -1);
// 	return (0);
// }
void print_tok_type(t_token_type type)
{
	if (type == TOK_WORD)
		printf("TOK_WORD");
	if (type == TOK_PIPE)
		printf("TOK_PIPE");
	if (type == TOK_REDIR_IN)
		printf("TOK_REDIR_IN");
	if (type == TOK_REDIR_OUT)
		printf("TOK_REDIR_OUT");
	if (type == TOK_APPEND)
		printf("TOK_APPEND");
	if (type == TOK_HEREDOC)
		printf("TOK_HEREDOC");
}
void print_tokens(t_token *token)
{
	t_token *next;

	next = token;
	while (next)
	{
		print_tok_type(next->type);
		printf(":%s\n", next->value);
		next = next->next;
	}
}

int	process_input(char *line, t_shell *shell)
{
	if (!line || !shell)
		return (-1);
	if (has_unclosed_quotes(line))
		return (print_syntax_error("unclosed quotes"), shell->last_status=-1, -1);
	if (shell->tokens)
		free_tokens(&(shell->tokens));
	shell->tokens = lex_input(line, shell);
	// print_tokens(shell->tokens);
	if (shell->tokens == NULL)
		return (shell->last_status=-1, -1);
	if (syntax_check_tokens(shell->tokens))
		return (free_tokens(&(shell->tokens)), shell->last_status=-1, -1);
	if (!expand_tokens(shell->tokens, shell))
		return (free_tokens(&(shell->tokens)), shell->last_status=-1, -1);
	if (shell->cmds)
		free_cmds(shell->cmds);
	shell->cmds = parse_tokens(shell->tokens, shell);
	if (!shell->cmds)
		return (shell->last_status=-1, -1);
	return (0);
}
