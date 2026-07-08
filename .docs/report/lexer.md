# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    lexer.txt                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/07 17:14:47 by ykonka            #+#    #+#              #
#    Updated: 2026/07/07 17:20:20 by ykonka           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### What it is?
- In computer science, a lexer (or tokenizer) is a program that breaks raw input text into a sequence of meaningful chunks called tokens.
- It performs the first phase of syntax analysis (called lexical analysis) before the text is passed to a parser.

### Step1:: t_token struct object splits and assigns user input as token elements of struct
e.g:: bash$ echo hello > out.txt | wc -c

0	TOK_WORD		echo
1	TOK_WORD		hello
2	TOK_REDIR_OUT	>
3	TOK_WORD		out.txt
4	TOK_PIPE		|
5	TOK_WORD		wc
6	TOK_WORD		-c

### Step2:: Lexer will split and store the input tokens into following structure
cmd1:
    argv   = ["echo", "hello", NULL]
    redirs = [{ type = R_OUT, file = "out.txt" }]
    next   = &cmd2

cmd2:
    argv   = ["wc", "-c", NULL]
    redirs = NULL
    next   = NULL



