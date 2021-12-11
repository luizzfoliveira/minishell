SRCS		=	src/checkers.c\
				src/execute_file.c\
				src/executor.c\
				src/minishell.c\
				src/parser.c\
				src/signals.c\
				src/substitute_vars.c\
				builtin/builtin_echo.c\
				builtin/builtin_exit.c\
				builtin/builtin_env.c\
				builtin/builtin_export.c\
				builtin/builtin_unset.c\
				gnl/get_next_line.c\
				gnl/get_next_line_utils.c\
				utils/ft_atoi.c\
				utils/ft_itoa.c\
				utils/ft_split.c\
				utils/fd_save.c\
				utils/utils.c


OBJS		= $(SRCS:.c=.o)

NAME		= minishell

CC			= gcc

CFLAGS		= -Wall -Werror -Wextra

LIBS		= -lreadline

.c.o:
			$(CC) -c $< -o $(<:.c=.o)

all:		$(NAME)

no_flags:	$(OBJS)
			$(CC) -o $(NAME) $(OBJS) $(LIBS)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

clean:
			rm -fr $(OBJS)

fclean:		clean
			rm -f $(NAME)

re:			fclean all
