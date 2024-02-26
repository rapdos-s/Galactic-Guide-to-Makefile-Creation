NAME = a.out
SOURCE = main.c hello_world.c
CC = cc
RM = rm -f

all: $(NAME)

$(NAME):
	$(CC) $(SOURCE) -o $(NAME)

clean:

fclean: clean
	$(RM) $(NAME)

re: fclean all
