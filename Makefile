SRCS		=	builtin_echo.c\
				checkers.c\
				env.c\
				execute_file.c\
				executor.c\
				ft_split.c\
				minishell.c\
				parser.c\
				signals.c\
				substitute_vars.c\
				utils.c\
				gnl/get_next_line.c\
				gnl/get_next_line_utils.c


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