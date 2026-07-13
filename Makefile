NAME                = minishell
TEST_NAME_LEXER     = lexer_tests
TEST_NAME_PARSER_ST1= parser_tests_stage1
TEST_NAME_PARSER_ST2= parser_tests_stage2
TEST_NAME_EXPANDER  = expander_tests
TEST_NAME_ENV       = env_tests

CC                  = cc
CFLAGS              = -Wall -Wextra -Werror
RM                  = rm -f
MKDIR_P             = mkdir -p

INCDIR              = include
OBJDIR              = obj
LIBFT_DIR           = libs/libft
LIBFT               = $(LIBFT_DIR)/libft.a
INCLUDES            = -I$(INCDIR) -I$(LIBFT_DIR)
LIBS                = -lreadline

SRCS                = src/1_lexer/lexer.c \
                      src/1_lexer/token_create.c \
                      src/1_lexer/lexer_utils.c \
                      src/1_lexer/token_utils.c \
                      src/2_parser/parser_utils.c \
                      src/2_parser/syntax_check.c \
                      src/2_parser/parser.c \
                      src/2_parser/parser_commands.c \
                      src/2_parser/parser_redirs.c \
                      src/3_expand/expand_utils.c \
                      src/3_expand/expand_status.c \
                      src/3_expand/expand_word.c \
                      src/3_expand/expand.c \
                      src/4_env/env_init.c \
                      src/4_env/env_utils.c \
                      src/4_env/env_to_array.c \
                      src/5_executor/exec_builtins.c \
                      src/5_executor/exec_child.c \
                      src/5_executor/exec_pipeline.c \
                      src/5_executor/exec.c \
                      src/5_executor/utils_path.c \
                      src/5_executor/utils.c \
                      src/6_builtins/cd.c \
                      src/6_builtins/echo.c \
                      src/6_builtins/env.c \
                      src/6_builtins/exit.c \
                      src/6_builtins/export.c \
                      src/6_builtins/pwd.c \
                      src/6_builtins/unset.c

MAIN_SRCS           = src/main/init.c \
                      src/main/main.c \
                      src/main/process_line.c \
                      src/main/cleanup.c \
                      src/main/signals.c \
                      src/main/utils.c

TEST_SRCS_LEXER     = tests/main_lexer_tests.c
TEST_SRCS_PARSER_ST1= tests/main_parser_tests_stage1.c
TEST_SRCS_PARSER_ST2= tests/main_parser_tests_stage2.c
TEST_SRCS_EXPANDER  = tests/main_expander_tests.c
TEST_SRCS_ENV       = tests/main_env_tests.c

OBJS                = $(SRCS:%.c=$(OBJDIR)/%.o)
MAIN_OBJS           = $(MAIN_SRCS:%.c=$(OBJDIR)/%.o)
TEST_OBJS_LEXER     = $(TEST_SRCS_LEXER:%.c=$(OBJDIR)/%.o)
TEST_OBJS_PARSER_ST1= $(TEST_SRCS_PARSER_ST1:%.c=$(OBJDIR)/%.o)
TEST_OBJS_PARSER_ST2= $(TEST_SRCS_PARSER_ST2:%.c=$(OBJDIR)/%.o)
TEST_OBJS_EXPANDER  = $(TEST_SRCS_EXPANDER:%.c=$(OBJDIR)/%.o)
TEST_OBJS_ENV       = $(TEST_SRCS_ENV:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(MAIN_OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(MAIN_OBJS) $(LIBFT) $(LIBS) -o $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJDIR)/%.o: %.c
	@$(MKDIR_P) $(@D)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_NAME_LEXER): $(OBJS) $(TEST_OBJS_LEXER) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS_LEXER) $(LIBFT) -o $(TEST_NAME_LEXER)

$(TEST_NAME_PARSER_ST1): $(OBJS) $(TEST_OBJS_PARSER_ST1) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS_PARSER_ST1) $(LIBFT) -o $(TEST_NAME_PARSER_ST1)

$(TEST_NAME_PARSER_ST2): $(OBJS) $(TEST_OBJS_PARSER_ST2) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS_PARSER_ST2) $(LIBFT) -o $(TEST_NAME_PARSER_ST2)

$(TEST_NAME_EXPANDER): $(OBJS) $(TEST_OBJS_EXPANDER) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS_EXPANDER) $(LIBFT) -o $(TEST_NAME_EXPANDER)

$(TEST_NAME_ENV): $(OBJS) $(TEST_OBJS_ENV) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS_ENV) $(LIBFT) -o $(TEST_NAME_ENV)

run: $(NAME)
	@./$(NAME)

run_tests: $(TEST_NAME_LEXER)
	@./$(TEST_NAME_LEXER)

run_tests_parser1: $(TEST_NAME_PARSER_ST1)
	@./$(TEST_NAME_PARSER_ST1)

run_tests_parser2: $(TEST_NAME_PARSER_ST2)
	@./$(TEST_NAME_PARSER_ST2)

run_tests_expander: $(TEST_NAME_EXPANDER)
	@./$(TEST_NAME_EXPANDER)

run_tests_env: $(TEST_NAME_ENV)
	@./$(TEST_NAME_ENV)

mem_check: $(TEST_NAME_LEXER)
	@valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		--log-file=valgrind-out.txt \
		./$(TEST_NAME_LEXER)

clean:
	@$(RM) -r $(OBJDIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@$(RM) $(NAME) $(TEST_NAME_LEXER) $(TEST_NAME_PARSER_ST1) $(TEST_NAME_PARSER_ST2) $(TEST_NAME_EXPANDER) $(TEST_NAME_ENV)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re run run_tests run_tests_parser1 run_tests_parser2 run_tests_expander run_tests_env mem_check
