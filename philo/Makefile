# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/26 13:28:07 by aalbrech          #+#    #+#              #
#    Updated: 2025/01/29 18:29:07 by aalbrech         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = main.c handle_input.c philo_nodes.c error.c \
threads.c utils.c philo_doing.c time.c philo_routine.c \
death_manager.c data_struct.c
		

OBJ = $(SRC:.c=.o)

NAME = philo

CC = cc

CFLAGS = -Wall -Wextra -Werror

.PHONY = all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)


