NAME = scop
DIR_SRCS = srcs
DIR_OBJS = bin

SRCS = ${shell bash ./scripts/generate_sources.sh}
OBJS = $(SRCS:%.cpp=$(DIR_OBJS)/%.o)

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++11 -MMD
CXXINCLUDES = -I/usr/include -I/usr/local/include ${shell bash ./scripts/generate_includes.sh}

LDFLAGS = -lglfw -lGLEW -lGL

MKDIR = mkdir -p
RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)


$(DIR_OBJS)/%.o : $(DIR_SRCS)/%.cpp
			${MKDIR} ${dir $@}
		${CXX} ${CXXFLAGS} ${CXXINCLUDES} -c $< -o $@

-include $(OBJS:.o=.d)

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME} ${DIR_OBJS}

re: fclean all

.PHONY: all clean fclean re
