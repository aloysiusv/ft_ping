# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/14 16:40:45 by lrandria          #+#    #+#              #
#    Updated: 2025/04/25 19:24:53 by lrandria         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	ft_ping

SRCS =	0_main.c \
		1_parser.c \
		2_ping.c \
		3_pong.c \
		utils_print.c \
		utils_stop.c \
		utils_calc.c \

INC =	ft_ping.h \

CC = gcc
FLAGS = -Wall -Wextra -Werror -g -fsanitize=address
LDFLAGS = -lm

OBJ = $(addprefix $(OBJ_DIR), $(SRCS:.$(SRCS_EXT)=.o))
DEP = $(addprefix $(DEP_DIR), $(SRCS:.$(SRCS_EXT)=.d))

SRCS_EXT = c
INC_EXT = h

SRCS_DIR =	./
INC_DIR =	./
OBJ_DIR =	./obj/
DEP_DIR =	./dep/

vpath    %.h $(INC_DIR)
vpath    %.o $(OBJ_DIR)
vpath    %.d $(DEP_DIR)

####

all: $(OBJ_DIR) $(DEP_DIR) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(LDFLAGS) $^ -o $@

clean: clean_dep clean_obj

fclean: clean clean_bin

re: fclean
	@make all

####

clean_dep:
	rm -rf $(DEP_DIR)

clean_obj:
	rm -rf $(OBJ_DIR)

clean_bin:
	rm -f $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(DEP_DIR):
	@mkdir -p $(DEP_DIR)

$(OBJ_DIR)%.o: %.$(SRCS_EXT)
	@mkdir -p $(OBJ_DIR)$(dir $<)
	@mkdir -p $(DEP_DIR)$(dir $<)
	$(CC) $(FLAGS) -MMD -MP -MF $(DEP_DIR)$*.d -c $< -o $@

####

-include $(DEP)

.PHONY: all clean fclean re
