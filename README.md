*This project has been created as part of the 42 curriculum by ykonka & ileongar*.

# 42_Heilbronn - Minishell Project

## Description

`minishell` is a simplified reimplementation of a Unix shell, written in C,
built as part of the 42 core curriculum. The goal of the project is to
understand, from the inside out, how a shell like `bash` actually works:
how a typed command line becomes a running process, how processes are
chained together with pipes, how input/output gets redirected to files,
and how a shell manages its own environment and built-in commands without
relying on any external shell library to do the heavy lifting.


Concretely, `minishell` implements:

- an interactive prompt with a working command **history**
- a lexer/parser that tokenizes a line, handles `'single'` and `"double"`
  quoting, and detects syntax errors around pipes and redirections
- **environment variable expansion** (`$VAR`) and **exit status expansion**
  (`$?`)
- **redirections**: `<` (input), `>` (output), `>>` (append), and `<<`
  (heredoc, reading until a delimiter line)
- **pipes** (`|`), chaining any number of commands together
- **process execution**: resolving executables via `$PATH` or a relative/
  absolute path, and running them with `fork`/`execve`
- **signal handling** that mirrors `bash` in interactive mode (`ctrl-C`,
  `ctrl-D`, `ctrl-\`)
- the mandatory **built-in commands**: `echo` (with `-n`), `cd`, `pwd`,
  `export`, `unset`, `env`, and `exit`

The project only needed to implement what a shell strictly needs to run
everyday command lines correctly — it intentionally does not attempt to
be a drop-in `bash` replacement (no scripting, no job control, no aliases,
no globbing beyond what the subject requires).

* Implementation done in `C` programming language

* For this, we were authorized to use the following functions:

	readline, rl_clear_history, rl_on_new_line,
	rl_replace_line, rl_redisplay, add_history,
	printf, malloc, free, write, access, open, read,
	close, fork, wait, waitpid, wait3, wait4, signal,
	sigaction, sigemptyset, sigaddset, kill, exit,
	getcwd, chdir, stat, lstat, fstat, unlink, execve,
	dup, dup2, pipe, opendir, readdir, closedir,
	strerror, perror, isatty, ttyname, ttyslot, ioctl,
	getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
	tgetnum, tgetstr, tgoto, tputs


## Instructions

### Requirements

- A C compiler (`cc`/`clang`/`gcc`)
- GNU `make`
- The `readline` development headers/library:
  ```bash
  sudo apt install libreadline-dev      # Debian/Ubuntu
  brew install readline                 # macOS
  ```

### Compilation

```bash
make          # builds the libft dependency, then the minishell binary
make clean    # removes object files
make fclean   # removes object files and the binary
make re       # fclean + all
```

The `Makefile` picks up every `.c` file under `src/` automatically, so
adding a new source file only means dropping it into the matching
subfolder — nothing in the `Makefile` itself needs to change.

### Execution

```bash
./minishell
```

### Usage examples

```bash
minishell$ echo "hello $USER, last exit code was $?"
minishell$ ls -la | grep ".c" | wc -l
minishell$ cat << EOF > out.txt
> some text with $HOME expanded
> EOF
minishell$ export PATH=$PATH:/extra/bin
minishell$ cd .. && pwd
minishell$ unset OLDPWD
minishell$ exit 0
```

## Resources
- `AI usage`: AI is used for research, test case generation and code refactorization.
- www.perplexity.com
- www.chatgpt.com
- www.claude.com

