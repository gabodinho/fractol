/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiertzu <ggiertzu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:57:54 by ggiertzu          #+#    #+#             */
/*   Updated: 2023/11/22 17:20:18 by ggiertzu         ###   ########.fr       */
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
#define THRESH 4
#ifndef MAXRUNS
# define MAXRUNS 500
#endif

typedef double	(*t_f)(double *input1, double *input2);

typedef	struct limits_s
{
	mlx_t	*window;
	mlx_image_t*	img;
	int		type;
	int		cmap;
	double	c[2];
	double	x_min;
	double	x_max;
	double	y_min;
	double	y_max;
	double	dx;
	double	dy;
}	t_limits;

typedef	struct input_s
{
	int	type;
	int	julia;
	int	cmap;
	int	success;
}	t_input;

double	mandel_step(double *prev, double *c)
{
	double	temp;

	temp = prev[0] * prev[0] - prev[1] * prev[1] + c[0];
	prev[1] = prev[0] * prev[1] * 2 + c[1];
	prev[0] = temp;
	return (prev[0] * prev[0] + prev[1] * prev[1]);
}

double	burning_step(double *prev, double *c)
{
	double	temp;

	temp = prev[0] * prev[0] - prev[1] * prev[1] + c[0];
	prev[1] = fabs(prev[0] * prev[1]) * 2 + c[1];
	prev[0] = temp;
	return (prev[0] * prev[0] + prev[1] * prev[1]);
}

int steps_mandel(double *point, t_f handler)
{
	int		i;
	double	res;
	double	prev[2];

	i = 0;
	res = 0;
	prev[0] = 0;
	prev[1] = 0;
	while (i++ < MAXRUNS && res < THRESH)
		res = handler(prev, point);
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

void	set_c(int type, int param, double *c)
{
	if (type == 2)
	{
		if (param == 1)
		{
			c[0] = 0.285;
			c[1] = 0.01;
		}
		else if (param == 2)
		{
			c[0] = -0.4;
			c[1] = 0.6;
		}
		else if (param == 3)
		{
			c[0] = -0.8;
			c[1] = 0.156;
		}
	}
}			

t_limits	init_lim(mlx_t *mlx, mlx_image_t *img, int type, int par, int cmap)
{
	t_limits	lim;

	lim.window= mlx;
	lim.img = img;
	lim.type = type;
	lim.cmap = cmap;
	set_c(type, par, lim.c);
	if (type == 1)
	{
		lim.x_min = -2.5;
		lim.x_max = 0.5;
		lim.y_min = -1.5;
		lim.y_max = 1.5;
	}
	else
	{
		lim.x_min = -2;
		lim.x_max = 2;
		lim.y_min = -2;
		lim.y_max = 2;
	}
	lim.dx = (lim.x_max - lim.x_min) / WIDTH;
	lim.dy = (lim.y_max - lim.y_min) / HEIGHT;
	return (lim);
}

int	get_steps(int i, int j, t_limits lim)
{
	double	point[2];

	point[0] = lim.x_min + (lim.dx * i);
	point[1] = lim.y_max - (lim.dy * j);
	if (lim.type == 1)
		return (steps_mandel(point, mandel_step));
	else if (lim.type == 2)
		return (steps_julia(point, lim.c));
	else
		return (steps_mandel(point, burning_step));

}

// black to red
int	cmap_def(int steps)
{
	uint8_t	red;
	uint8_t	green;

	if (steps >= MAXRUNS)
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

	if (steps >= MAXRUNS)
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

void	draw_image(mlx_image_t *img, t_limits lim)
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
			colour = get_colour(get_steps(i, j, lim), lim.cmap);	// +++ param für cmap
			put_colour((img -> pixels) + pixel_idx, colour);
			j++;
		}
		i++;
	}
}

void	move_lim(t_limits *lim, int direc)
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

void	reset_view(t_limits *lim)
{
	if (lim -> type == 1)
	{
		lim -> x_min = -2.5;
		lim -> x_max = 0.5;
		lim -> y_min = -1.5;
		lim -> y_max = 1.5;
	}
	else if (lim -> type >= 2)
	{
		lim -> x_min = -2;
		lim -> x_max = 2;
		lim -> y_min = -2;
		lim -> y_max = 2;
	}
	// keep in mind burning ship
	lim -> dx = (lim -> x_max - lim -> x_min) / WIDTH;
	lim -> dy = (lim -> y_max - lim -> y_min) / HEIGHT;
	draw_image(lim -> img, *lim);
	mlx_image_to_window(lim -> window, lim -> img, 0, 0);
}

void	move_view(void *input)
{
	t_limits	*lim;
	mlx_t	*mlx;

	lim = (t_limits *) input;
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

void	adapt_lim(t_limits *lim, double rate)
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
	t_limits	*lim;

	lim = (t_limits *) param;
	(void) xdelta;
	if (ydelta > 0)
		adapt_lim(lim, 2);
	else if (ydelta < 0)
		adapt_lim(lim, 0.5);
}

void	put_error(int code)
{
	if (code < 3)
	{
		if (code == 1)
			printf("Invalid parameter(s)\n");
		else if (code == 2)
			printf("missing/too many parameter(s)\n");
		printf(" 1st param: 1=mandelbrot, 2=julia, 3=burning ship\n");
		printf(" [2nd param] (julia points): from 1 to 3, def=1\n");
		printf(" [3rd param] (colour map): from 1 to 2, def=1\n");
	}
	else
		printf("mlx error");
	exit(EXIT_FAILURE);

}

void	check_semantic(t_input res)
{
	if (res.type > 0 && res.type < 4)
	{
		if (res.julia > 0 && res.julia < 4)
		{
			if (res.cmap > 0 && res.cmap < 3)
				return;
		}
	}
	put_error(1);
}		

t_input	check_input(int32_t argc, const char *argv[])
{
	t_input	res;

	if (argc > 1 && argc < 5)
	{
		res.type = atoi(argv[1]);		//------------------change for ft_atoi !!
		res.julia = 1;
		res.cmap = 1;
	}
	else
		put_error(2);
	if (argc >= 3)
		res.julia = atoi(argv[2]);
	if (argc == 4)
		res.cmap = atoi(argv[3]);
	check_semantic(res);
	return (res);
}

int32_t main(int32_t argc, const char* argv[])
{
	mlx_t* mlx;
	mlx_image_t* image;
	t_limits	lim;
	t_input		input;

	input = check_input(argc, argv);
	mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true);
	image = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!mlx || !image)
		put_error(3);
	lim = init_lim(mlx, image, input.type, input.julia, input.cmap);
	draw_image(image, lim);
    mlx_image_to_window(mlx, image, 0, 0);
	mlx_loop_hook(mlx, move_view, &lim);
	mlx_scroll_hook(mlx, zoom_hook, &lim);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
