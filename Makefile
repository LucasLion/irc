# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llion <llion@student.42mulhouse.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/06 01:09:23 by llion             #+#    #+#              #
#    Updated: 2023/10/05 14:30:12 by llion@student    ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ircserv
CXX		= c++
CXXFLAGS= -ggdb3 -Wall -Wextra -Werror -std=c++98 -Wshadow
RM		= rm -f
SRC		= server.cpp
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
	@./$(NAME)

run: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)
	@./$(NAME)

clean:
	$(RM) $(OBJ) $(TOBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re run debug
