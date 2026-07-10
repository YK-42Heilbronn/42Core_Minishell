# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    parser.txt                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/08 12:06:27 by ykonka            #+#    #+#              #
#    Updated: 2026/07/08 12:06:28 by ykonka           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### What is it?
- a parser is an software component in computer program which takes sequential data (such as raw text or stream of tokens) and breaks them down into structured, hierarchical format.
- By analysing the syntax against a set of rules(grammer), it organizes the data so a computer can easily understand and process it.
- summary: input -> arranges(break and build data structure) -> structured format (tree or linked list or array) -> analyses (checks grammer rule adherence) -> keep ready for computer to execute
- parsers are composed of two main parts:
	+ lexical analysis
	+ syntaxical analysis

### What each file does
Each parser file should stay focused on one responsibility, which is exactly what the 42 Norm encourages.

- quote_check.c: only scans raw input for unmatched quotes.
- syntax_check.c: only validates token order.
- parser.c: high-level parser flow.
- parser_commands.c: builds argv and command nodes.
- parser_redirs.c: builds redirection nodes and attaches them to commands.

### Suggested dependency flow
This is the call chain you should aim for:

- has_unclosed_quotes(line)
- lex_input(line, shell)
- syntax_check_tokens(tokens)
- parse_tokens(tokens, shell)
	- parse_pipeline(&tokens, shell)
		- parse_command(&tokens, shell)
			- parse_redirection(cmd, &tokens)

### parse_command()
#### For this input token list:

[TOK_WORD "echo"] -> [TOK_WORD "hi"] -> [TOK_REDIR_OUT ">"]
-> [TOK_WORD "out.txt"] -> [TOK_PIPE "|"] -> [TOK_WORD "wc"]

#### parse_command(&tokens, shell) should build something like:

cmd->argv   = ["echo", "hi", NULL];
cmd->redirs = [R_OUT, "out.txt"];
cmd->next   = NULL;