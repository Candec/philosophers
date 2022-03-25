NAME = philo

CFLAGS = -Wall -Wextra -Werror -pthread -g -fsanitize=thread
AR = ar rcsv
OBJ_DIR = obj
SRC_DIR = src
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

HEADER = philo.h

SRC = check.c \
	ft_atoi.c \
	life.c \
	philo.c \
	utils.c \
	parallel_life.c \

all: $(NAME)

$(NAME): $(OBJ)
	@echo $(ANSI_B_BGREEN) "compiling executable" $(ANSI_RESET)$(ANSI_F_BBLACK)
	gcc $(CFLAGS) $(OBJ) -o $(NAME)
	@echo $(ANSI_RESET) ""

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(addprefix include/,$(HEADER))
	@echo $(ANSI_B_BGREEN) "compiling $(NAME) objects" $(ANSI_RESET)$(ANSI_F_BBLACK)
	mkdir -p $(dir $@)
	gcc $(CFLAGS) -Iinclude -c $< -o $@
	@echo $(ANSI_RESET)

clean:
	@echo $(ANSI_B_RED) "clean" $(ANSI_RESET)$(ANSI_F_BRED)
	rm -rf $(OBJ_DIR)
	@echo $(ANSI_RESET) ""

fclean: clean
	@echo $(ANSI_B_RED) "fclean" $(ANSI_RESET)$(ANSI_F_BRED)
	rm -f $(NAME)
	@echo $(ANSI_RESET) ""

re: fclean all

.PHONY: all clean fclean

# colors
ANSI_RESET = "\033[0m"
ANSI_B_RED = "\033[41m"
ANSI_B_BGREEN = "\033[42;1m"
ANSI_F_BRED = "\033[31;1m"
ANSI_F_BBLACK = "\033[30;1m"


norm:
	@echo $(ANSI_B_RED) "norminette v3" $(ANSI_RESET)
	@norminette $(addprefix include/,$(HEADER)) \
		$(addprefix src/,$(SRC))

run: all
	@echo $(ANSI_B_RED) "Running for debbuger without sanitize" $(ANSI_RESET)
	./$(NAME) $(LIST_INTEGER)
	@./$(NAME) $(LIST_INTEGER) | wc -l
	@./$(NAME) $(LIST_INTEGER) | ./checker $(LIST_INTEGER)

runv: all
	@echo $(ANSI_B_RED) "Valgrind RESULT" $(ANSI_RESET)
	#valgrind -q --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME)
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(NAME)

runs: all
	@echo $(ANSI_B_RED) "Running with sanitize" $(ANSI_RESET)
	gcc $(CFLAGS) -fsanitize=address $(OBJ) $(LIBFT) -o pipex
	./push_swaps $(LIST_INTEGER)
