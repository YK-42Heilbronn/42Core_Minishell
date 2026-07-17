/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_lexer_tests.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 12:29:23 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/08 19:02:07 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include "lexer.h"
#include "parser.h"

static const char *token_name(t_token_type type)
{
    if (type == TOK_WORD)
        return ("WORD");
    if (type == TOK_PIPE)
        return ("PIPE");
    if (type == TOK_REDIR_IN)
        return ("REDIR_IN");
    if (type == TOK_REDIR_OUT)
        return ("REDIR_OUT");
    if (type == TOK_HEREDOC)
        return ("HEREDOC");
    if (type == TOK_APPEND)
        return ("APPEND");
    return ("UNKNOWN");
}

static void print_tokens(t_token *list)
{
    while (list)
    {
        if (list->type == TOK_WORD)
            printf("%s(%s)\n", token_name(list->type), list->value);
        else
            printf("%s(%s)\n", token_name(list->type), list->value);
        list = list->next;
    }
}

static void run_one_test(const char *input, t_shell *shell, int index)
{
    t_token *tokens;

    printf("==============================\n");
    printf("TEST %d\n", index);
    printf("INPUT: [%s]\n", input);
    printf("TOKENS:\n");
    tokens = lex_input(input, shell);
    if (!tokens)
        printf("(null)\n");
    else
        print_tokens(tokens);
    free_tokens(tokens);
    printf("\n");
}

int main(void)
{
    t_shell     shell;
    const char  *tests[] = {
        "ls",
        "echo hello",
        "pwd",
        "ls -la /tmp",
        "ls | wc -l",
        "cat < in.txt",
        "echo hi > out.txt",
        "echo hi >> log.txt",
        "cat << EOF",
        "grep foo < in.txt > out.txt",
        "echo \"hello world\"",
        "echo 'hello world'",
        "echo \"a b\"'c d'",
        "echo hello|cat",
        "echo hi>file",
        "cat<<EOF",
        "a>b<c",
        "cmd1|cmd2|cmd3",
        "echo \"$USER\"",
        "echo '$USER'",
        "   ls",
        "echo    hello",
        "   echo   hello   |   cat   ",
        "\techo\t\tok",
        "| ls",
        "ls |",
        "echo >",
        "echo >>",
        "<< EOF",
        "echo \"unterminated",
        "echo 'unterminated",
        NULL
    };
    int         i;

    shell.env = NULL;
    shell.last_status = 0;
    shell.tokens = NULL;
    shell.cmds = NULL;
    i = 0;
    while (tests[i])
    {
        run_one_test(tests[i], &shell, i + 1);
        i++;
    }
    return (0);
}
