NAME        = minishell
TEST_NAME   = lexer_tests
CC          = cc
CFLAGS      = -Wall -Wextra -Werror
RM          = rm -f
MKDIR_P     = mkdir -p

INCDIR      = include
OBJDIR      = obj
LIBFT_DIR   = libs/libft
LIBFT       = $(LIBFT_DIR)/libft.a
INCLUDES    = -I$(INCDIR) -I$(LIBFT_DIR)

SRCS        = src/lexer/lexer.c \
              src/lexer/token_create.c \
              src/lexer/lexer_utils.c \
			  src/lexer/token_utils.c

TEST_SRCS   = tests/main_lexer_tests.c

OBJS        = $(SRCS:%.c=$(OBJDIR)/%.o)
TEST_OBJS   = $(TEST_SRCS:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJDIR)/%.o: %.c
	@$(MKDIR_P) $(@D)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_NAME): $(OBJS) $(TEST_OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(TEST_OBJS) $(LIBFT) -o $(TEST_NAME)

run_tests: $(TEST_NAME)
	./$(TEST_NAME)

clean:
	@$(RM) -r $(OBJDIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@$(RM) $(NAME) $(TEST_NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re run_tests
