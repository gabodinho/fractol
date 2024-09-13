# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ggiertzu <ggiertzu@student.42berlin.d      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/22 19:51:00 by ggiertzu          #+#    #+#              #
#    Updated: 2024/01/26 22:43:28 by ggiertzu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= fractol
#	RUNS	?= 100
#	CFLAGS	:= -g -O0 -D MAXRUNS=$(RUNS) -Wall -Werror -Wextra -pthread -Ofast
CFLAGS  := -g -O0 -Wall -Werror -Wextra -pthread -Ofast
CC		:= gcc
LIBMLX_D:= ./MLX42/build
LIBMLX_L:= libmlx42.a
LIBMLX	:= $(LIBMLX_D)/$(LIBMLX_L)
LIBFT_D	:= ./libft
LIBFT_L	:= libftprintf.a
LIBFT	:= $(LIBFT_D)/$(LIBFT_L)


HEADERS	:= -I MLX42/include -I $(LIBFT_D) -I .
LIBS	:= $(LIBMLX) $(LIBFT) -ldl -lglfw -lm
SRCS	:= fractol_main.c fractol_utils1.c fractol_utils2.c fractol_utils3.c \
			fractol_utils4.c
OBJS	:= ${SRCS:.c=.o}

all: $(NAME)

bonus: all

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): $(LIBFT) $(LIBMLX) $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

$(LIBMLX):
	@if [ ! -d MLX42 ]; then \
		echo "Downloading miniLibX..."; \
		git clone https://github.com/codam-coding-college/MLX42.git; \
		cd MLX42; cmake -B build; cmake --build build -j4; \
	fi

$(LIBFT):
	make -C $(LIBFT_D)
clean:
	rm -rf $(OBJS)
	make clean -C $(LIBFT_D)
fclean: clean
	rm -rf $(NAME)
	rm -rf MLX42
	make fclean -C $(LIBFT_D)
re: clean all

.PHONY: all, clean, fclean, re, libmlx
