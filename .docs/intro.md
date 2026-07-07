# Minishell


## Parsing
ykonka

## Execution and Process Management
ileongar

### Task List::

* Pipe mechanism
* Redirection
* Basic shell commands implementation from scratch
* &&, || and wildcard entries - bonus(excluded)

// FYI - Info from ChatGPT:

The biggest mistake students make: many students start with execution.
That is usually a disaster. The recommended architecture used by many successful 42 students is:

Input
 ↓
Lexer
 ↓
Parser
 ↓
Expander
 ↓
Executor
