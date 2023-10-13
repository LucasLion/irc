
include includes/help.mk

NAME	= ircserv
COMMAND	= $(NAME) 6667 test
CXX		= c++
CXXFLAGS= -ggdb3 -Wall -Wextra -Werror -std=c++98 -Wshadow
RM		= rm -f
SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

all: $(NAME)

$(OBJ_DIR)/.dummy: # Create obj directory
	mkdir -p $(OBJ_DIR)
	touch $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR)/.dummy # Compile .cpp files
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJECTS) # Compile executable
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJECTS)


leaks: $(OBJECTS) # Compile executable with valgrind
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJECTS)
	@valgrind --leak-check=full --show-leak-kinds=all ./$(COMMAND)

debug: $(OBJECTS) # Compile executable with gdb (-fsanitize=address)
	$(CXX) $(CXXFLAGS) -fsanitize=address -o $(NAME) $(OBJECTS)
	@./$(NAME) 6667 test

run: $(OBJECTS) # Compile executable and run it
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJECTS)
	@./$(COMMAND)

clean: # Remove object files and obj directory
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/.dummy
	rmdir $(OBJ_DIR)

fclean: clean # Remove executable, obj directory and object files
	$(RM) $(NAME)

re: fclean all # Recompile everything

.PHONY: all clean fclean re run debug leaks
