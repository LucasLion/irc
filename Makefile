
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

$(OBJ_DIR)/.dummy:
	mkdir -p $(OBJ_DIR)
	touch $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR)/.dummy
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJECTS)

leaks: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJECTS)
	@valgrind --leak-check=full --show-leak-kinds=all ./$(COMMAND)

debug: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -fsanitize=address -o $(NAME) $(OBJECTS)
	@./$(NAME) 6667 test

run: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJECTS)
	@./$(COMMAND)

clean:
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/.dummy
	rmdir $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re run debug
