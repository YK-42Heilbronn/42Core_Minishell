### Shell
- shell is an program and an single process(grouped subprocesses).
- which does the tasks like interacting with and manipulating the file system and os system processes.

### Data structures of Minishell Project and Their Relationship among them

- t_shell is the shell session, but tokens and cmds are current-input state
This is another useful refinement. t_shell represents the running shell session, but shell->tokens and shell->cmds are usually the data for the current command line being processed, not the entire session history. So:

	+ env, last_status, stdin_fd, stdout_fd, exit_flag are session-level state.
	+ line, tokens, cmds are current-input processing state.

- That distinction will help a lot once you start freeing and rebuilding data for each new prompt line.

#### This is the clearest way to think about it:

						`user input line
							↓
						lexer
							↓
						t_token linked list
							↓
						parser
							↓
						t_cmd linked list
							↓
						executor`
`And around all of that, t_shell is the container that holds the shell’s current state`

### Example: Making Understanding More Solid
Take this input:
`$bash> grep main < infile | wc -l > outfile`

#### Token level: The lexer should produce a linked list like

						TOK_WORD("grep")

						TOK_WORD("main")

						TOK_REDIR_IN("<")

						TOK_WORD("infile")

						TOK_PIPE("|")

						TOK_WORD("wc")

						TOK_WORD("-l")

						TOK_REDIR_OUT(">")

						TOK_WORD("outfile").

#### Command level: The parser should build two t_cmd nodes
##### First t_cmd
						argv = ["grep", "main", NULL]

						redirs = [R_IN, "infile"]

##### Second t_cmd
						argv = ["wc", "-l", NULL]

						redirs = [R_OUT, "outfile"]

`And cmd1->next = cmd2`

