/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parser_tests_stage1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 17:54:09 by ykonka            #+#    #+#             */
/*   Updated: 2026/07/09 17:54:12 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdio.h>

typedef struct s_quote_test
{
	const char	*input;
	int			expected;
}				t_quote_test;

static void	run_one_test(t_quote_test test, int i)
{
	int	result;

	result = has_unclosed_quotes(test.input);
	printf("Test %d\n", i + 1);
	printf("Input    : %s\n", test.input);
	printf("Expected : %s\n", test.expected ? "invalid" : "valid");
	printf("Got      : %s\n", result ? "invalid" : "valid");
	if (result == test.expected)
		printf("Result   : PASS\n\n");
	else
		printf("Result   : FAIL\n\n");
}

int	main(void)
{
	t_quote_test	tests[] = {{"echo hello", 0}, {"echo \"hello\"", 0},
			{"echo 'hello'", 0}, {"echo \"hello world\"", 0},
			{"echo 'hello world'", 0}, {"echo \"'hello'\"", 0},
			{"echo '\"hello\"'", 0}, {"echo \"hello 'world'\"", 0},
			{"echo 'hello \"world\"'", 0}, {"\"abc\"", 0}, {"'abc'", 0},
			{"echo \"\"", 0}, {"echo ''", 0}, {"echo \"hello", 1},
			{"echo 'hello", 1}, {"\"", 1}, {"'", 1}, {"echo \"abc", 1},
			{"echo 'abc", 1}, {"echo \"abc'", 1}, {"echo 'abc\"", 1}, {"\"'",
			1}, {"'\"", 1}};
	int				i;
	int				count;

	count = sizeof(tests) / sizeof(tests[0]);
	i = 0;
	while (i < count)
	{
		run_one_test(tests[i], i);
		i++;
	}
	return (0);
}
