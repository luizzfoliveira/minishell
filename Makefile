SRCS		= ft_calloc.c\
			  ft_strncmp.c\
			  gnl/get_next_line.c\
			  gnl/get_next_line_utils.c\
			  minishell.c\
			  signals.c

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