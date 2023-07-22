NAME := scop
UNAME := $(shell uname)
ARCH := $(shell uname -m)

DIR_SRCS := srcs
DIR_OBJS := bin
DIR_GLFW := ./libs/GLFW
DIR_GLEW := ./libs/GLEW
DIR_STB := ./libs/STB

SRCS := $(shell bash ./scripts/generate_sources.sh)
OBJS := $(SRCS:%.cpp=$(DIR_OBJS)/%.o)

CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -pedantic -std=c++11 -MMD
CXXINCLUDES := -I$(DIR_GLFW)/include/ -I$(DIR_GLEW)/include/ -I$(DIR_STB)/include/ $(shell bash ./scripts/generate_includes.sh)

ifeq ($(UNAME), Linux)
CXXDEPENDENCIES := -L$(DIR_GLFW)/lib/Linux -lglfw3 -L$(DIR_GLEW)/lib/Linux -lGLEW -lGL -lX11
else ifeq ($(UNAME), Darwin)
ifeq ($(ARCH), arm64)
CXXDEPENDENCIES := -L$(DIR_GLFW)/lib/Darwin -lglfw3 -L$(DIR_GLEW)/lib/Darwin -lGLEW -framework OpenGL -framework Cocoa -framework IOKit
else
$(error Unsupported architecture: ${ARCH}, scop only supports arm64 on macOs.)
endif
else
$(error Unsupported os: ${UNAME}, scop only supports Linux)
endif

MKDIR := mkdir -p
RM := rm -rf

RED := \033[1;31m
GREEN := \033[1;32m
CYAN := \033[1;36m
NC := \033[0;37m

COMPILING := $(CYAN)Compiling$(NC)
LINKING := $(GREEN)Linking$(NC)
REMOVING := $(RED)Removing$(NC)

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

.PHONY: all clean fclean re






