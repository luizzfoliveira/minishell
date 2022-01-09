SRCS		=	src/checkers.c\
				src/execute_file_utils.c\
				src/execute_file_utils2.c\
				src/execute_file.c\
				src/executor.c\
				src/get_args.c\
				src/minishell.c\
				src/parser_utils.c\
				src/parser_utils2.c\
				src/parser.c\
				src/redirects.c\
				src/save_env_var.c\
				src/signals.c\
				src/signals_2.c\
				src/sintax_check.c\
				src/substitute_vars.c\
				src/substitute_vars_utils.c\
				builtin/builtin_echo.c\
				builtin/builtin_exit.c\
				builtin/builtin_export_utils.c\
				builtin/builtin_export.c\
				builtin/builtin_unset.c\
				builtin/builtin_cd.c\
				builtin/builtin_pwd.c\
				builtin/builtin_env.c\
				utils/cleanup.c\
				utils/ft_atoi.c\
				utils/ft_itoa.c\
				utils/ft_split.c\
				utils/fd_save.c\
				utils/utils_0.c\
				utils/utils_1.c\
				utils/utils_2.c\
				utils/error.c\
				utils/heredoc.c


OBJS		= $(SRCS:.c=.o)

NAME		= minishell

CC			= gcc

CFLAGS		= -Wall -Werror -Wextra

LIBS		= -lreadline

.c.o:
			$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

all:		$(NAME)

no_flags:	$(OBJS)
			$(CC) -o $(NAME) $(OBJS) $(LIBS)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

debug:
			$(CC) $(CFLAGS) -g3 -o minishell_debug $(SRCS) $(LIBS)

clean:
			rm -fr $(OBJS)

fclean:		clean
			rm -f $(NAME)

re:			fclean all
