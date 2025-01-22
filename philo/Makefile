# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/26 13:28:07 by aalbrech          #+#    #+#              #
#    Updated: 2025/01/22 13:26:34 by aalbrech         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = main.c handle_input.c init.c error.c philo_time.c utils.c philo_doing.c time.c
		

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


