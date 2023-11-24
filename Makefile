# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ggiertzu <ggiertzu@student.42berlin.d      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/22 19:51:00 by ggiertzu          #+#    #+#              #
#    Updated: 2023/11/24 09:08:08 by ggiertzu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= fractol
#CFLAGS	:= -Wextra -Wall -Werror -Wunreachable-code -Ofast
CFLAGS	:= -g -O0 -Wall -Werror -Wextra
CC		:= gcc
LIBMLX	:= ../../MLX42

HEADERS	:= -I ./include -I $(LIBMLX)/include -I ./libft -I .
LIBS	:= $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm -L./libft/ -lftprintf
SRCS	:= fractol_main.c fractol_utils1.c fractol_utils2.c fractol_utils3.c \
			fractol_utils4.c
OBJS	:= ${SRCS:.c=.o}

all: $(NAME)

#libmlx:
#	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

clean:
	@rm -rf $(OBJS)
#	@rm -rf $(LIBMLX)/build

fclean: clean
	@rm -rf $(NAME)

re: clean all

.PHONY: all, clean, fclean, re, libmlx
