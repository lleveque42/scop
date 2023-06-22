NAME= scop

DIR_SRCS = srcs
DIR_OBJS = bin
DIR_GLFW = ./libs/GLFW
DIR_GLEW = ./libs/GLEW

SRCS = ${shell bash ./scripts/generate_sources.sh}
OBJS = $(SRCS:%.cpp=$(DIR_OBJS)/%.o)

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++11 -MMD
CXXINCLUDES = -I${DIR_GLFW}/include/ -I${DIR_GLEW}/include/ ${shell bash ./scripts/generate_includes.sh}

CXXDEPENDENCIES = -L$(DIR_GLFW)/lib -lglfw3 -L${DIR_GLEW}/lib64 -lGLEW -lGL -lX11 -lOpenGL

MKDIR = mkdir -p
RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(CXXDEPENDENCIES)


$(DIR_OBJS)/%.o : $(DIR_SRCS)/%.cpp
			${MKDIR} ${dir $@}
		${CXX} ${CXXFLAGS} -c $< -o $@ ${CXXINCLUDES}

-include $(OBJS:.o=.d)

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME} ${DIR_OBJS}

re: fclean all

.PHONY: all clean fclean re
