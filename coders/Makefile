NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
# -pthread es crucial: le dice al compilador que enlace 
# la librería de hilos POSIX (libpthread), sin ella, 
# funciones como pthread_create no compilarán

SRCS = main.c \
		utils.c \
		heap.c \
		init.c \
		routine.c \
		arbiter.c \
		monitor.c \
		dongles.c \
		clean.c

OBJ_DIR		= obj

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: %.c codexion.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJS)
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re