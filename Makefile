NAME := scop
UNAME := ${shell uname}

DIR_SRCS := srcs
DIR_OBJS := bin
DIR_GLFW := ./libs/GLFW
DIR_GLEW := ./libs/GLEW

SRCS := $(shell bash ./scripts/generate_sources.sh)
OBJS := $(SRCS:%.cpp=$(DIR_OBJS)/%.o)

CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -pedantic -std=c++11 -MMD
CXXINCLUDES := -I$(DIR_GLFW)/include/ -I$(DIR_GLEW)/include/ $(shell bash ./scripts/generate_includes.sh)

CXXDEPENDENCIES := -L$(DIR_GLFW)/lib/Linux -lglfw3 -L$(DIR_GLEW)/lib/Linux -lGLEW -lGL -lX11

MKDIR := mkdir -p
RM := rm -rf

RED := \033[1;31m
GREEN := \033[1;32m
CYAN := \033[1;36m
NC := \033[0;37m

COMPILING := $(CYAN)Compiling$(NC)
LINKING := $(GREEN)Linking$(NC)
REMOVING := $(RED)Removing$(NC)

ifeq ($(UNAME), Linux)
all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(LINKING) scop..."
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(CXXDEPENDENCIES)

$(DIR_OBJS)/%.o : $(DIR_SRCS)/%.cpp
	@$(MKDIR) $(dir $@)
	@echo "$(COMPILING) $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@ $(CXXINCLUDES)

-include $(OBJS:.o=.d)

clean:
	@echo "$(REMOVING) objects..."
	@$(RM) $(OBJS) $(DIR_OBJS)

fclean: clean
	@echo "$(REMOVING) executable..."
	@$(RM) $(NAME)

re: fclean all
else
$(error Unsupported os: ${UNAME}, scop only supports Linux)
endif

.PHONY: all clean fclean re
