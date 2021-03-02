# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maquentr <maquentr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/02 13:05:22 by maquentr          #+#    #+#              #
#    Updated: 2021/03/02 13:07:41 by maquentr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=printf

CC=gcc

CFLAGS=-Wall -Wextra

RM=rm -f

INC=-I./ft_printf.h

SRC=ft_printf.c

OBJ=$(SRC:.c=.o)


$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(SRC) $(INC) -o $(NAME)

all: $(NAME)


clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME) 

re: fclean all

.PHONY: clean fclean
