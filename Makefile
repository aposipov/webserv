
NAME	=	webserv
CC		=	c++
W_FLAGS	=	-Wall -Wextra -Werror
S_FLAGS =	-std=c++98
INC_FLAG=	-I ./inc/
RM		=	rm -rf
OBJ 	=	$(patsubst %.cpp, %.o, $(SRC))

.PHONY:		all clean fclean re test

RED= \033[1;31m
GREEN= \033[1;32m
NC= \033[0;0m


INC		=	inc/Server.hpp inc/Conf.hpp inc/Client.hpp inc/Request.hpp inc/Response.hpp
SRC		=	src/main.cpp src/Server.cpp src/Conf.cpp src/Client.cpp src/Request.cpp src/Response.cpp

all:		$(NAME)

$(NAME):	$(OBJ)
			$(CC) $(W_FLAGS) $(S_FLAGS) $(OBJ) -o $(NAME)
			@echo -e "$(GREEN)\t\n $(NAME) compiled!\n $(NC)"

%.o : %.cpp	Makefile $(INC)
			$(CC) $(INC_FLAG) $(W_FLAGS) $(S_FLAGS) -c $< -o $@

client:
			c++ ./test/tcpclient.cpp -o client

client_clean:
			rm client
			@echo -e "$(RED)client $(NC) file is delete!$(NC)"

clean:
			$(RM) $(OBJ)
			@echo -e "$(RED)*.o files is clean!$(NC)"

fclean:		clean
			$(RM) $(NAME)
			@echo -e "$(RED)$(NAME) file is delete!$(NC)"

re:			fclean all

test:		$(NAME)
			valgrind ./$(NAME)