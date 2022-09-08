# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smun <smun@student.42seoul.kr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/08 16:12:02 by smun              #+#    #+#              #
#    Updated: 2022/09/08 20:02:18 by smun             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX			:= c++
RM			:= rm -f
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++2a
CXXDEBUG	:= -g3 -fsanitize=address

INCDIR		:= inc
OBJDIR		:= obj
SRCDIR		:= src
SDL2_INC	:= $(shell sdl2-config --cflags)
SDL2_LIB	:= $(shell sdl2-config --static-libs)
VK_INC		:= -I$(shell brew --prefix molten-vk)/include
VK_LIB		:= $(shell brew --prefix molten-vk)/lib/libMoltenVK.a

INC			:= $(SDL2_INC) $(VK_INC) -I$(INCDIR)
LIB			:= $(SDL2_LIB) $(VK_LIB)

FILES		:= \
				std.cpp \
				main.cpp

SRCS		:= $(addprefix $(SRCDIR)/, $(FILES))
OBJS		:= $(addprefix $(OBJDIR)/, $(FILES:.cpp=.o))
DEPS		:= $(addprefix $(OBJDIR)/, $(FILES:.cpp=.d))

TARGET		:= scop

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CXXDEBUG) $(INC) $(LIB) -o $@ $^

clean:
	$(RM) $(DEPS)
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(TARGET)

re: fclean all

-include $(DEPS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CXXDEBUG) $(INC) -MMD -c -o $@ $<

.PHONY: all clean fclean re
