NAME = a.out

SOURCE = main.c hello_world.c
OBJECTS = $(SOURCE:%.c=%.o)
DEPENDENCIES = $(OBJECTS:%.o=%.d)

CC = cc
RM = rm -f

.DEFAULT_GOAL = all

all: $(NAME)

clean:
	$(RM) $(DEPENDENCIES)
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(NAME)

%.o: %.c
	$(CC) -c $< -o $@ -MMD

-include $(DEPENDENCIES)
