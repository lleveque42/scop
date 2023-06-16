NAME = scop
DIR_SRCS = srcs
DIR_OBJS = bin

SRCS = main.cpp parsing/checks.cpp
OBJS = $(SRCS:%.cpp=$(DIR_OBJS)/%.o)

CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -I/usr/include -I/usr/local/include -MMD
LDFLAGS = -lglfw -lGLEW -lGL

MKDIR = mkdir -p
RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)


$(DIR_OBJS)/%.o : $(DIR_SRCS)/%.cpp
			${MKDIR} ${dir $@}
		${CXX} ${CXXFLAGS} -c $< -o $@

-include $(OBJS:.o=.d)

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME} ${DIR_OBJS}

re: fclean all

.PHONY: all clean fclean re
