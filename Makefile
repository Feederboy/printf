# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maquentr <maquentr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/02 13:05:22 by maquentr          #+#    #+#              #
#    Updated: 2021/03/02 13:45:04 by maquentr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME=libftprintf.a

CC=gcc

CFLAGS=-Wall -Wextra -Werror

RM=rm -f

INC=ft_printf.h

SRC=ft_printf.c

OBJ=$(SRC:.c=.o)


%.o: %.c
	$(CC) $(CFLAGS) -c $< $(INC)

$(NAME): $(OBJ)
	ar rcs	$(NAME)	$(OBJ)

all: $(NAME)

bonus:
	@make	all	OBJ="$(OBJ)"

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean
