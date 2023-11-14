/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiertzu <ggiertzu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:57:54 by ggiertzu          #+#    #+#             */
/*   Updated: 2023/11/14 22:03:33 by ggiertzu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <MLX42/MLX42.h>
#include <string.h>

#define WIDTH 256
#define HEIGHT 256
#ifndef THRESH
# define THRESH 4
# endif
#ifndef MAXRUNS
# define MAXRUNS 20
# endif

//
//	----- input from utils ----
//
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
// check input in the beginning
	while (i < MAXRUNS && res < THRESH)
	{
		res = mandel_step(prev, point);
//		printf("%f\n", res);
		i++;
	}
//	printf("%d\n", i);
	return i;
}
//
//	----- end utils -----
//

typedef	struct limits_s
{
	double	x_min;
	double	x_max;
	double	y_min;
	double	y_max;
	double	delta_x;
	double	delta_y;
}	limits_t;

limits_t	init_lim(void)
{
	limits_t	lim;

	lim.x_min = -2.5;
	lim.x_max = 0.5;
	lim.y_min = -1.5;
	lim.y_max = 1.5;
	lim.delta_x = (lim.x_max - lim.x_min) / WIDTH;
	lim.delta_y = (lim.y_max - lim.y_min) / HEIGHT;
	return (lim);
}

int	get_steps(int i, int j, limits_t lim)
{
	double	point[2];

	point[0] = lim.x_min + (lim.delta_x * i);
	point[1] = lim.y_max - (lim.delta_y * j);
	return (steps_mandel(point));
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

int	get_colour(int steps, int cmap)
{
	if (cmap == 1)
		return (cmap_def(steps));
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
			colour = get_colour(get_steps(i, j, lim), 1);
			put_colour((img -> pixels) + pixel_idx, colour);
//			printf("%d %d %d %d\n", img -> pixels[pixel_idx], img->pixels[pixel_idx+1], img->pixels[pixel_idx+2],img->pixels[pixel_idx+3]);
			j++;
		}
		i++;
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
	lim = init_lim();
	draw_image(image, lim);
    mlx_image_to_window(mlx, image, 0, 0);
//	mlx_loop_hook(mlx, ft_hook, mlx);

	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
