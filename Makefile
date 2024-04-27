CXX = c++
#CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 
RM		= rm -f
NAME	= webserv
SRC		= message.cpp\
			request.cpp\
			response.cpp\
			config.cpp\
			cgi.cpp\

OBJ	= $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME)

clean:
		$(RM) $(OBJ)

fclean: clean
		$(RM) $(NAME)

re:	fclean all