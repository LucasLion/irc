
NAME	= ircserv
CXX		= c++
CXXFLAGS= -ggdb3 -Wall -Wextra -Werror -std=c++98 -Wshadow
RM		= rm -f
SRC		= main.cpp Server.cpp User.cpp Command.cpp
OBJ		= $(SRC:.cpp=.o)

all: $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ) 
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

leaks: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)
	make clean
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

debug: $(OBJ)
	$(CXX) $(CXXFLAGS) -fsanitize=address -o $(NAME) $(OBJ)
	@./$(NAME) 6667 test

run: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)
	@./$(NAME)

clean:
	$(RM) $(OBJ) $(TOBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re run debug
