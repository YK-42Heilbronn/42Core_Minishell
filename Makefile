NAME                = minishell

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
                      src/1_lexer/token.c \
                      src/1_lexer/lexer_utils.c \
                      src/1_lexer/token_utils.c \
                      src/2_parser/parser_utils.c \
                      src/2_parser/syntax_check.c \
                      src/2_parser/parser.c \
                      src/2_parser/parser_commands.c \
                      src/2_parser/parser_redirs.c \
                      src/3_expand/expand_utils.c \
                      src/3_expand/expand_word_utils.c \
                      src/3_expand/expand_word.c \
                      src/3_expand/expand.c \
                      src/4_env/env_init.c \
                      src/4_env/env_utils.c \
                      src/4_env/env_to_array.c \
                      src/5_executor/exec_builtins.c \
                      src/5_executor/exec_path.c \
                      src/5_executor/exec_pipeline.c \
                      src/5_executor/exec.c \
                      src/5_executor/heredoc.c \
                      src/5_executor/redirections.c \
                      src/5_executor/utils_heredoc.c \
                      src/5_executor/utils_path.c \
                      src/5_executor/utils_pipeline.c \
                      src/6_builtins/cd.c \
                      src/6_builtins/echo.c \
                      src/6_builtins/env.c \
                      src/6_builtins/exit.c \
                      src/6_builtins/export.c \
                      src/6_builtins/pwd.c \
                      src/6_builtins/unset.c

MAIN_SRCS           = src/main/main.c \
                      src/main/signals.c \
                      src/main/utils.c

OBJS                = $(SRCS:%.c=$(OBJDIR)/%.o)
MAIN_OBJS           = $(MAIN_SRCS:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(MAIN_OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(MAIN_OBJS) $(LIBFT) $(LIBS) -o $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJDIR)/%.o: %.c
	@$(MKDIR_P) $(@D)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(RM) -r $(OBJDIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
