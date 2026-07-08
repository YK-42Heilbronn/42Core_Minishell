
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
7   TOK_REDIR_IN    <
8	TOK_APPEND      >>
9	TOK_HEREDOC     <<
10	TOK_EOF         

### Step2:: Lexer will split and store the input tokens into following structure
cmd1:
    argv   = ["echo", "hello", NULL]
    redirs = [{ type = R_OUT, file = "out.txt" }]
    next   = &cmd2

cmd2:
    argv   = ["wc", "-c", NULL]
    redirs = NULL
    next   = NULL



