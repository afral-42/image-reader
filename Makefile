CC=cc
CFLAGS=-Wall -Wextra -Werror
CPPFLAGS=-Iincludes
DEPS_FLAGS=-MMD -MP

SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LIBS := $(shell sdl2-config --libs)

SRC_DIR=srcs/
BUILD_DIR=.build/

SRC_FILES=main.c parsing.c

SRC=$(addprefix $(SRC_DIR), $(SRC_FILES))
OBJS=$(addprefix $(BUILD_DIR), $(patsubst %.c, %.o, $(SRC_FILES)))
DEPS=$(patsubst %.o, %.d, $(OBJS))

NAME=img_reader

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(SDL2_LIBS) -o $@

$(BUILD_DIR)%.o: $(SRC_DIR)%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPS_FLAGS) -c $< -o $@ $(SDL2_CFLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug:
	make re "CFLAGS=-Wall -Wextra -Werror -g"

.PHONY: all clean fclean re debug

-include $(DEPS)