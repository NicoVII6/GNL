NAME = get_next_line.a

SRC = get_next_line.c get_next_line_utils.c \

OBJ = $(SRC:.c=.o)

CC = gcc

CFLAGS += -Wall -Wextra -Werror

AR = ar

RM = rm -f

all : $(NAME)

$(NAME) : $(OBJ)
		$(AR) -rcs $(NAME) $(OBJ)

%.o : %.c
		$(CC) -I. $(CFLAGS) -o $@ -c $?

clean :
		$(RM) $(OBJ)

fclean : clean
		$(RM) $(NAME)

re : fclean all

.PHONY : clean fclean re