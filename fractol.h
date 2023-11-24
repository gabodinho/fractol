/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiertzu <ggiertzu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:57:54 by ggiertzu          #+#    #+#             */
/*   Updated: 2023/11/24 08:56:09 by ggiertzu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <math.h>
# include <MLX42/MLX42.h>
# include <string.h>
# include <ft_printf.h>
# define WIDTH 512
# define HEIGHT 512
# define THRESH 4
# ifndef MAXRUNS
#  define MAXRUNS 500
# endif

typedef double	(*t_f)(double *input1, double *input2);

typedef struct limits_s
{
	mlx_t		*window;
	mlx_image_t	*img;
	int			type;
	int			cmap;
	double		c[2];
	double		x_min;
	double		x_max;
	double		y_min;
	double		y_max;
	double		dx;
	double		dy;
}	t_limits;

typedef struct input_s
{
	int	type;
	int	julia;
	int	cmap;
	int	success;
}	t_input;

//static double		mandel_step(double *prev, double *c);
//static double		burning_step(double *prev, double *c);
//static int			steps_mandel(double *point, t_f handler);
//static int			steps_julia(double *point, double *c);
//static void		set_c(int type, int param, double *c);
t_limits	init_lim(mlx_t *mlx, mlx_image_t *img, t_input input);
int			get_steps(int i, int j, t_limits lim);
//static int			cmap_def(int steps);
//static int			cmap_advanced(int steps);
//static int			get_colour(int steps, int cmap);
//static void		put_colour(uint8_t *pixels, int rgba);
void		draw_image(mlx_image_t *img, t_limits lim);
//static void		move_lim(t_limits *lim, int direc);
//static void		reset_view(t_limits *lim);
void		move_view(void *input);
//static void		adapt_lim(t_limits *lim, double rate);
void		zoom_hook(double xdelta, double ydelta, void *param);
void		put_error(int code);
//static void		check_semantic(t_input res);
t_input		check_input(int32_t argc, const char *argv[]);
#endif