
NAME = Gomoku

CC = gcc
OPTFLAG= -O3 -lm -lpthread -g


SRCS_PATH = srcs/
SRCS_NAME =		brain.c			neuron.c	tools.c		config.c	\
				graphique.c		id.c		player.c	game.c		\
				apply_move.c	train.c		mutate.c	ia0.c		\
				ia1.c			ia2.c
				
SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))


OBJS_PATH = objs/
OBJS_NAME = $(SRCS_NAME:.c=.o)
OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))


INCLUDES_PATH = includes/
INCLUDES = $(addprefix -I , $(INCLUDES_PATH))

all: $(NAME) 

$(NAME): obj
	$(CC) srcs/main.c $(OBJS) $(OPTFLAG) -o $(NAME) $(INCLUDES)
	
obj :
	$(CC) $(OPTFLAG) -c $(SRCS) $(INCLUDES)
	@mv *.o objs/
	
clean:
	rm -rf $(OBJS)
	
fclean: clean
	rm -rf $(NAME)
	
re: clean all