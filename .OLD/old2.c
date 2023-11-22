/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiertzu <ggiertzu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:57:54 by ggiertzu          #+#    #+#             */
/*   Updated: 2023/11/19 15:19:37 by ggiertzu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <MLX42/MLX42.h>
#include <string.h>

#define WIDTH 512
#define HEIGHT 512
#ifndef THRESH
# define THRESH 4
# endif
#ifndef MAXRUNS
# define MAXRUNS 100
# endif

void	comp_sq(double *comp)
{
	double	real;
	double	img;

	if (!comp)
		return;
	real = comp[0];
	img = comp[1];
	comp[0] = real * real - img * img;
	comp[1] = real * img * 2;
}

void	comp_sum(double *comp1, double *comp2)
{
	if (!comp1 || !comp2)
		return;
	comp1[0] = comp1[0] + comp2[0];
	comp1[1] = comp1[1] + comp2[1];
}

double	comp_abs(double *comp)
{
	double	res;

	if (!comp)
		return (-1);
	res = comp[0] * comp[0]  + comp[1] * comp[1];
	return res;
}

void	printc(double *comp)
{
	printf("real: %f img: %f\n", comp[0], comp[1]);
}

double	mandel_step(double *prev, double *c)
{
	if (!prev || !c)
		return (-1);
	comp_sq(prev);
	comp_sum(prev, c);
	return (comp_abs(prev));
}

int steps_mandel(double *point)
{
	int	i;
	double	res;
	double	prev[2];

	i = 0;
	res = -1;
	prev[0] = 0;
	prev[1] = 0;
	while (i < MAXRUNS && res < THRESH)
	{
		res = mandel_step(prev, point);
		i++;
	}
	return i;
}

int steps_julia(double *point, double *c)
{
	int	i;
	double	res;

	i = 0;
	res = -1;
	while (i++ < MAXRUNS && res < THRESH)
		res = mandel_step(point, c);
	return i;
}

typedef	struct limits_s
{
	mlx_t	*window;
	mlx_image_t*	img;
	int		type;
	double	c[2];
	double	x_min;
	double	x_max;
	double	y_min;
	double	y_max;
	double	dx;
	double	dy;
}	limits_t;

void	set_c(int type, int param, double *c)
{
	if (type == 2)
	{
		if (param == 0)
		{
			c[0] = 0.285;
			c[1] = 0.01;
		}
		else if (param == 1)
		{
			c[0] = -0.4;
			c[1] = 0.6;
		}
		else if (param == 2)
		{
			c[0] = -0.8;
			c[1] = 0.156;
		}
	}
	else
	{
		c[0] = 0;
		c[1] = 0;
	}
}			

limits_t	init_lim(mlx_t *mlx, mlx_image_t *image, int type, int param)
{
	limits_t	lim;

	lim.window= mlx;
	lim.img = image;
	lim.type = type;
	set_c(type, param, lim.c);
	if (type == 1)
	{
		lim.x_min = -2.5;
		lim.x_max = 0.5;
		lim.y_min = -1.5;
		lim.y_max = 1.5;
	}
	else
	{
		lim.x_min = -1.5;
		lim.x_max = 1.5;
		lim.y_min = -1.5;
		lim.y_max = 1.5;
	}
	lim.dx = (lim.x_max - lim.x_min) / WIDTH;
	lim.dy = (lim.y_max - lim.y_min) / HEIGHT;
	return (lim);
}

int	get_steps(int i, int j, limits_t lim)
{
	double	point[2];

	point[0] = lim.x_min + (lim.dx * i);
	point[1] = lim.y_max - (lim.dy * j);
	if (lim.type == 1)
		return (steps_mandel(point));
	else
		return (steps_julia(point, lim.c));
}

// black to red
int	cmap_def(int steps)
{
	uint8_t	red;
	uint8_t	green;

	if (steps == MAXRUNS)
	{
		green = 0;
		red = 0;
	}
	else if (MAXRUNS / 2 <= steps)
	{
		green = 0;
		red = -410 * steps / MAXRUNS + 460;
	}
	else
	{
		red = 255;
		green = 255 * 2 * (MAXRUNS - steps) / MAXRUNS;
	}
	return ((red << 24) | (green << 16) | (0x00 << 8) | 0xFF);
}

int	cmap_advanced(int steps)
{
	uint8_t	red;
	uint8_t	green;

	if (steps == MAXRUNS)
	{
		green = 0;
		red = 0;
	}
	else if (MAXRUNS / 2 <= steps)
	{
		green = 255;
		red = 410 * steps / MAXRUNS - 205;
	}
	else
	{
		red = 0;
		green = 510 * steps / MAXRUNS;
	}
	return ((red << 24) | (green << 16) | (0x00 << 8) | 0xFF);
}

int	get_colour(int steps, int cmap)
{
	if (cmap == 1)
		return (cmap_def(steps));
	else if (cmap == 2)
		return (cmap_advanced(steps));
	return (-1);
}

void	put_colour(uint8_t *pixels, int rgba)
{
	if (!pixels)
		return ;
	*pixels = (rgba >> 24) & 0xFF;
	*(pixels+1) = (rgba >> 16) & 0xFF;
	*(pixels+2) = (rgba >> 8) & 0xFF;
	*(pixels+3) = (rgba) & 0xFF;
	return ;
}

void	draw_image(mlx_image_t *img, limits_t lim)
{
	int i;
	int	j;
	int	pixel_idx;
	int colour;
	
	i = 0;
	pixel_idx = 0;
	colour = 0;
	while (i < WIDTH)
	{
		j = 0;
		while (j < HEIGHT)
		{
			pixel_idx = (j * WIDTH * 4) + (i * 4);
			colour = get_colour(get_steps(i, j, lim), 1);			// +++ param für cmap
			put_colour((img -> pixels) + pixel_idx, colour);
			j++;
		}
		i++;
	}
}

void	move_lim(limits_t *lim, int direc)
{
	if (direc == 1)
	{
		lim -> y_max += lim -> dy * HEIGHT / 4;
		lim -> y_min += lim -> dy * HEIGHT / 4;
	}
	else if (direc == 2)
	{
		lim -> x_max += lim -> dx * WIDTH / 4;
		lim -> x_min += lim -> dx * WIDTH / 4;
	}
	else if (direc == 3)
	{
		lim -> y_max -= lim -> dy * HEIGHT / 4;
		lim -> y_min -= lim -> dy * HEIGHT / 4;
	}
	else
	{
		lim -> x_max -= lim -> dx * WIDTH / 4;
		lim -> x_min -= lim -> dx * WIDTH / 4;
	}
	lim -> dx = (lim -> x_max - lim -> x_min) / WIDTH;
	lim -> dy = (lim -> y_max - lim -> y_min) / HEIGHT;
	draw_image(lim -> img, *lim);
	mlx_image_to_window(lim -> window, lim -> img, 0, 0);
}

void	reset_view(limits_t *lim)
{
	if (lim -> type == 1)
	{
		lim -> x_min = -2.5;
		lim -> x_max = 0.5;
		lim -> y_min = -1.5;
		lim -> y_max = 1.5;
	}
	else if (lim -> type == 2)
	{
		lim -> x_min = -1.5;
		lim -> x_max = 1.5;
		lim -> y_min = -1.5;
		lim -> y_max = 1.5;
	}
	lim -> dx = (lim -> x_max - lim -> x_min) / WIDTH;
	lim -> dy = (lim -> y_max - lim -> y_min) / HEIGHT;
	draw_image(lim -> img, *lim);
	mlx_image_to_window(lim -> window, lim -> img, 0, 0);
}

void	move_view(void *input)
{
	limits_t	*lim;
	mlx_t	*mlx;

	lim = (limits_t *) input;
	mlx = lim -> window;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_SPACE))
		reset_view(lim);
	if (mlx_is_key_down(mlx, MLX_KEY_UP))
		move_lim(lim, 1);
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
		move_lim(lim, 3);
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
		move_lim(lim, 4);
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
		move_lim(lim, 2);
}

void	adapt_lim(limits_t *lim, double rate)
{
	double	dx_n;
	double	dy_n;
	int32_t	x_pos;
	int32_t	y_pos;
	
	dx_n = lim -> dx / rate;
	dy_n = lim -> dy / rate;
	mlx_get_mouse_pos(lim -> window, &x_pos, &y_pos);
	y_pos = HEIGHT - y_pos;
	lim -> x_max = lim -> x_min + x_pos * lim -> dx + (WIDTH - x_pos) * dx_n;
	lim -> x_min = lim -> x_max - WIDTH * dx_n;
	lim -> y_max = lim -> y_min + y_pos * lim -> dy + (HEIGHT - y_pos) * dy_n;
	lim -> y_min = lim -> y_max - HEIGHT * dy_n;
	lim -> dx = dx_n;
	lim -> dy = dy_n;
	draw_image(lim -> img, *lim);
	mlx_image_to_window(lim -> window, lim -> img, 0, 0);
}

void zoom_hook(double xdelta, double ydelta, void* param)
{
	limits_t	*lim;

	lim = (limits_t *) param;
	if (ydelta > 0)
	{
		adapt_lim(lim, 2);
		printf("up\n");
	}
	else if (ydelta < 0)
	{
		adapt_lim(lim, 0.5);
		printf("down\n");
	}
}

int32_t main(int32_t argc, const char* argv[])
{
	mlx_t* mlx;
	mlx_image_t* image;
	limits_t	lim;

	// Gotta error check this stuff
	if (!(mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (!(image = mlx_new_image(mlx, WIDTH, HEIGHT)))
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (mlx_image_to_window(mlx, image, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	lim = init_lim(mlx, image, 1, 2);			// +++ param 1: type; param2: julia_c
	draw_image(image, lim);
    mlx_image_to_window(mlx, image, 0, 0);
	mlx_loop_hook(mlx, move_view, &lim);
	mlx_scroll_hook(mlx, zoom_hook, &lim);

	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
