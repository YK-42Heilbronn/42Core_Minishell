NAME                = minishell
TEST_NAME_LEXER     = lexer_tests
TEST_NAME_PARSER_ST1= parser_tests_stage1
TEST_NAME_PARSER_ST2= parser_tests_stage2

CC                  = cc
CFLAGS              = -Wall -Wextra -Werror
RM                  = rm -f
MKDIR_P             = mkdir -p

INCDIR              = include
OBJDIR              = obj
LIBFT_DIR           = libs/libft
LIBFT               = $(LIBFT_DIR)/libft.a
INCLUDES            = -I$(INCDIR) -I$(LIBFT_DIR)

SRCS                = src/lexer/lexer.c \
                      src/lexer/token_create.c \
                      src/lexer/lexer_utils.c \
                      src/lexer/token_utils.c \
                      src/parser/quote_check.c \
                      src/parser/syntax_check.c \
                      src/parser/parser.c \
                      src/parser/parser_commands.c \
                      src/parser/parser_redirs.c
#                     src/main/init.c \
#                     src/main/main.c

TEST_SRCS_LEXER     = tests/main_lexer_tests.c
TEST_SRCS_PARSER_ST1= tests/main_parser_tests_stage1.c
TEST_SRCS_PARSER_ST2= tests/main_parser_tests_stage2.c

OBJS                = $(SRCS:%.c=$(OBJDIR)/%.o)
TEST_OBJS_LEXER     = $(TEST_SRCS_LEXER:%.c=$(OBJDIR)/%.o)
TEST_OBJS_PARSER_ST1= $(TEST_SRCS_PARSER_ST1:%.c=$(OBJDIR)/%.o)
TEST_OBJS_PARSER_ST2= $(TEST_SRCS_PARSER_ST2:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

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

run_tests: $(TEST_NAME_LEXER)
	./$(TEST_NAME_LEXER)

run_tests_parser1: $(TEST_NAME_PARSER_ST1)
	./$(TEST_NAME_PARSER_ST1)

run_tests_parser2: $(TEST_NAME_PARSER_ST2)
	./$(TEST_NAME_PARSER_ST2)

mem_check: $(TEST_NAME_LEXER)
	valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		--log-file=valgrind-out.txt \
		./$(TEST_NAME_LEXER)

clean:
	@$(RM) -r $(OBJDIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@$(RM) $(NAME) $(TEST_NAME_LEXER) $(TEST_NAME_PARSER_ST1) $(TEST_NAME_PARSER_ST2)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re run_tests run_tests_parser1 run_tests_parser2 mem_check

