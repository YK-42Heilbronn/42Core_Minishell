## Development Plan

This project targets the **mandatory part only** of `minishell`, which requires a shell with a prompt, history, PATH-based execution, quote handling, environment variable expansion, exit status expansion, redirections, pipes, signals, and the required builtins.[file:16]

### Scope

The implementation must cover only the mandatory requirements listed in the subject:[file:16]

- Prompt display while waiting for a command.[file:16]
- Working history through `readline` support.[file:16]
- Execution of binaries using `PATH`, relative paths, or absolute paths.[file:16]
- Quote handling: single quotes block interpretation of metacharacters, and double quotes block interpretation except for `$` expansion.[file:16]
- Redirections: `<`, `>`, `<<`, `>>`.[file:16]
- Pipes using `|`.[file:16]
- Environment variable expansion with `$VAR` and exit status expansion with `$?`.[file:16]
- Interactive signal behavior for `ctrl-C`, `ctrl-D`, and `ctrl-\` as described in the subject.[file:16]
- Builtins: `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`.[file:16]

The repository must also include a `Makefile`, source files in C, headers, and a `README.md` in English, while respecting the Norm and avoiding crashes or memory leaks from the project code.[file:16]

### Team Split

The work is split into exactly two parts so both members can progress in parallel.

#### Person A — Parsing and Shell State

Person A owns everything from user input up to the final command representation:[file:16]

- Readline loop integration support with command preprocessing.[file:16]
- Lexer and token list creation.[file:16]
- Syntax validation for pipes, redirections, and invalid command shapes.[file:16]
- Quote handling rules.[file:16]
- Expansion of `$VAR` and `$?`.[file:16]
- Internal command structures passed to the executor.
- Parent-side builtins that modify shell state: `cd`, `export`, `unset`, `exit`.[file:16]
- Shell environment list structure and update logic.

#### Person B — Execution and Process Control

Person B owns everything from command execution onward:[file:16]

- External command execution with `fork`, `execve`, and `waitpid`.[file:16]
- PATH resolution.
- Pipe setup and teardown.
- Redirection handling for `<`, `>`, `>>`, and `<<`.[file:16]
- Heredoc reading flow.
- Interactive and execution-time signal handling.[file:16]
- Child process file descriptor management.
- Builtins that do not need to change parent shell state directly: `echo`, `pwd`, `env`.[file:16]

### Timeline

The schedule below is designed for **5 days**, with **9 to 10 hours of work per day**.

#### Day 1 — Architecture and Skeleton

**Main goal:** start a working shell skeleton and freeze interfaces early.

**Person A**
- Read the mandatory section and extract a feature checklist directly from the subject.[file:16]
- Define core structs: shell state, token, command, redirection, environment node.
- Create lexer token types for words, pipes, and redirections.
- Implement basic quote checking for unclosed quotes.[file:16]

**Person B**
- Set up repository, folders, `Makefile`, include paths, and `libft` integration.[file:16] (done)
- Implement `main`(done), initialization (done), prompt display, and `readline` loop.
- Copy `envp` into an internal environment structure.
- Create a minimal executor for one simple external command.

**Shared checkpoint**
- `./minishell` compiles and starts.
- Prompt appears.
- Input is read correctly.
- History is recorded.
- `ctrl-D` exits cleanly.[file:16]

#### Day 2 — Lexer, Parser, and Single Command Execution

**Main goal:** convert input into valid command structures and run single commands.

**Person A**
- Finish lexer behavior around spaces, quotes, `|`, `<`, `>`, `<<`, and `>>`.
- Build parser output for one or more commands.
- Add syntax checks for invalid token sequences.
- Preserve quote semantics for later expansion and execution.[file:16]

**Person B**
- Implement PATH search and executable resolution.[file:16]
- Detect builtins versus external commands.
- Implement `echo -n`, `pwd`, and `env`.[file:16]
- Track and store exit status after each command.

**Shared checkpoint**
- Single external commands work.
- Simple builtins work.
- Errors do not crash the shell.[file:16]

#### Day 3 — Expansion and Parent Builtins

**Main goal:** finish shell-state logic and variable processing.

**Person A**
- Implement `$VAR` expansion.[file:16]
- Implement `$?` expansion.[file:16]
- Apply correct expansion behavior inside single and double quotes.[file:16]
- Integrate expansion into the parser-to-executor flow.

**Person B**
- Implement `cd`, `export`, `unset`, and `exit` in the correct parent-process context.[file:16]
- Finalize environment update helpers.
- Convert internal env list to `char **` for `execve`.
- Improve error reporting paths.

**Shared checkpoint**
- `export TEST=42` then `echo $TEST` works.
- `unset TEST` works.
- `cd` changes the working directory of the shell.
- `$?` reflects the last foreground pipeline status.[file:16]

#### Day 4 — Redirections, Pipes, and Signals

**Main goal:** complete the core shell execution features.

**Person A**
- Finalize parser support for multiple redirections per command.
- Ensure parser output preserves command and redirection order.
- Support heredoc delimiter storage in command structures.

**Person B**
- Implement `<`, `>`, `>>`, and `<<` using the allowed system calls.[file:16]
- Implement pipelines with `pipe`, `fork`, `dup2`, and process synchronization.[file:16]
- Implement interactive signal behavior:
  - `Ctrl-C` prints a new prompt on a new line.[file:16]
  - `Ctrl-D` exits the shell.[file:16]
  - `Ctrl-\` does nothing in interactive mode.[file:16]
- Separate parent and child signal behavior.

**Shared checkpoint**
- Redirections work on single commands.
- Multiple commands connected by pipes work.
- Pipes and redirections work together.
- Heredoc works for mandatory behavior.[file:16]

#### Day 5 — Stabilization, Norm, README, and Defense Prep

**Main goal:** make the mandatory part stable and submission-ready.

**Person A**
- Stress-test parsing, quotes, expansions, empty input, and syntax errors.
- Audit memory ownership for tokens, commands, and environment nodes.
- Write the README planning and project description sections in English.[file:16]

**Person B**
- Stress-test execution, missing files, command-not-found cases, permissions, and bad redirections.
- Audit file descriptor closing and restoration.
- Finalize signal and exit status behavior.

**Shared checkpoint**
- Run `make re` successfully.[file:16]
- Check the Norm.
- Verify no crashes and no leaks from project-owned memory.[file:16]
- Do one mock peer evaluation where each teammate explains both parsing and execution.

### Daily Working Rhythm

A stable daily rhythm helps keep the team synchronized:

- **09:00–09:30** — daily planning and interface decisions.
- **09:30–12:30** — deep work block 1.
- **12:30–13:15** — lunch break.
- **13:15–16:15** — deep work block 2.
- **16:15–16:45** — merge, compile, and quick regression tests.
- **16:45–18:30** — bug fixing or integration work.
- **18:30–19:00** — update notes and define the next day’s tasks.

### Project Structure

A clean structure reduces merge conflicts and keeps responsibilities clear:

```text
minishell/
├── Makefile
├── README.md
├── include/
│   ├── minishell.h
│   ├── parser.h
│   ├── executor.h
│   ├── builtins.h
│   ├── signals.h
│   └── env.h
├── src/
│   ├── main/
│   │   ├── main.c
│   │   ├── shell_init.c
│   │   └── shell_loop.c
│   ├── lexer/
│   │   ├── lexer.c
│   │   ├── lexer_utils.c
│   │   ├── token_create.c
│   │   └── token_utils.c
│   ├── parser/
│   │   ├── parser.c
│   │   ├── parser_commands.c
│   │   ├── parser_redirs.c
│   │   ├── syntax_check.c
│   │   └── quote_check.c
│   ├── expand/
│   │   ├── expand.c
│   │   ├── expand_env.c
│   │   ├── expand_status.c
│   │   └── expand_utils.c
│   ├── exec/
│   │   ├── executor.c
│   │   ├── executor_pipeline.c
│   │   ├── executor_redirections.c
│   │   ├── executor_path.c
│   │   ├── child_process.c
│   │   └── parent_process.c
│   ├── builtins/
│   │   ├── builtin_echo.c
│   │   ├── builtin_cd.c
│   │   ├── builtin_pwd.c
│   │   ├── builtin_export.c
│   │   ├── builtin_unset.c
│   │   ├── builtin_env.c
│   │   ├── builtin_exit.c
│   │   └── builtin_utils.c
│   ├── signals/
│   │   ├── signals_interactive.c
│   │   └── signals_exec.c
│   ├── env/
│   │   ├── env_init.c
│   │   ├── env_get.c
│   │   ├── env_set.c
│   │   ├── env_unset.c
│   │   └── env_to_array.c
│   ├── utils/
│   │   ├── error.c
│   │   ├── free.c
│   │   ├── string_utils.c
│   │   └── fd_utils.c
│   └── debug/
│       └── print_debug.c
├── libft/
└── tests/
    ├── input_cases.txt
    ├── builtins_cases.txt
    ├── pipe_redir_cases.txt
    └── signals_cases.txt
```

### Files and Naming Conventions

The codebase should follow one consistent naming rule from the beginning:

- File names use lowercase with underscores, such as `parser_redirs.c` or `builtin_export.c`.
- Header names follow the same style, such as `executor.h` or `env.h`.
- Struct names use the `t_` prefix, such as `t_shell`, `t_token`, `t_cmd`, `t_redir`, and `t_env`.
- Enum names use the `e_` prefix, such as `e_token_type` and `e_redir_type`.
- Function names start with the module name, such as `lex_input`, `parse_tokens`, `expand_vars`, `exec_pipeline`, `env_set_value`, and `bi_export`.
- Macro names stay uppercase, such as `SUCCESS`, `ERROR`, or `TOKEN_WORD`.
- Only one global variable should exist, and it should store only the received signal number, as required by the subject.[file:16]
- Test files should end with `_cases.txt` for clarity.

### Merge and Collaboration Rules

To keep development smooth over five days:

- Person A owns parsing structures and expansion flow.
- Person B owns execution flow and process management.
- Shared headers are changed only after both teammates agree.
- Every merge must compile before it is pushed.
- Every implemented feature should come with at least three manual test commands.
- No bonus features should be added before the mandatory part is fully stable, because the subject evaluates the bonus only if the mandatory part is perfect.[file:16]

### Mandatory Completion Checklist

The mandatory part can be treated as complete only when all of the following are working:[file:16]

- [ ] Prompt
- [ ] History
- [ ] PATH/relative/absolute execution
- [ ] Single quotes
- [ ] Double quotes with `$` behavior
- [ ] `$VAR`
- [ ] `$?`
- [ ] `<`, `>`, `<<`, `>>`
- [ ] Pipes
- [ ] `ctrl-C`, `ctrl-D`, `ctrl-\`
- [ ] `echo -n`
- [ ] `cd`
- [ ] `pwd`
- [ ] `export`
- [ ] `unset`
- [ ] `env`
- [ ] `exit`
- [ ] Required `Makefile`
- [ ] English `README.md`
- [ ] No crashes, no obvious leaks, and no non-mandatory features

