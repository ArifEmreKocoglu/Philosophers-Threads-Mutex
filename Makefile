SRCS	= philosophers.c

SRCS	+= philosophers_utility.c

OBJS	= $(SRCS:.c=.o)

NAME	= philo

CLANG	= clang

FLAGS	= -Wall -Wextra -Werror

INCLUDE	= -lpthread

HEADER	= philo.h




all:	$(NAME)

.PHONY:	clean fclean re bonus bench bclean

$(NAME): $(OBJS)
	$(CLANG) $(INCLUDE) $(FLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

%.o: %.c $(HEADER)
	$(CLANG) $(FLAGS) -c $<  -o $(<:.c=.o)
