/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiertzu <ggiertzu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 18:23:51 by ggiertzu          #+#    #+#             */
/*   Updated: 2023/11/24 08:58:57 by ggiertzu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>

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

t_limits	init_lim(mlx_t *mlx, mlx_image_t *img, t_input input)
{
	t_limits	lim;

	lim.window = mlx;
	lim.img = img;
	lim.type = input.type;
	lim.cmap = input.cmap;
	set_c(lim.type, input.julia, lim.c);
	if (lim.type == 1)
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
	lim -> dx = (lim -> x_max - lim -> x_min) / WIDTH;
	lim -> dy = (lim -> y_max - lim -> y_min) / HEIGHT;
	draw_image(lim -> img, *lim);
	mlx_image_to_window(lim -> window, lim -> img, 0, 0);
}

void	move_view(void *input)
{
	t_limits	*lim;
	mlx_t		*mlx;

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
