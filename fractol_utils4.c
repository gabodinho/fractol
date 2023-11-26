/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiertzu <ggiertzu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 18:26:21 by ggiertzu          #+#    #+#             */
/*   Updated: 2023/11/24 17:18:19 by ggiertzu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>

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

void	zoom_hook(double xdelta, double ydelta, void *param)
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
			ft_printf("Invalid parameter(s)\n");
		else if (code == 2)
			ft_printf("missing/too many parameter(s)\n");
		ft_printf(" 1st param: 1=mandelbrot, 2=julia, 3=burning ship\n");
		ft_printf(" [2nd param] (julia points): from 1 to 3, def=1\n");
		ft_printf(" [3rd param] (colour map): from 1 to 2, def=1\n");
	}
	else
		ft_printf("mlx error");
	exit(EXIT_FAILURE);
}

void	check_semantic(t_input res)
{
	if (res.type > 0 && res.type < 4)
	{
		if (res.julia > 0 && res.julia < 4)
		{
			if (res.cmap > 0 && res.cmap < 3)
				return ;
		}
	}
	put_error(1);
}

t_input	check_input(int32_t argc, const char *argv[])
{
	t_input	res;

	if (argc > 1 && argc < 5)
	{
		res.type = ft_atoi(argv[1]);
		res.julia = 1;
		res.cmap = 1;
	}
	else
		put_error(2);
	if (argc >= 3)
		res.julia = ft_atoi(argv[2]);
	if (argc == 4)
		res.cmap = ft_atoi(argv[3]);
	check_semantic(res);
	return (res);
}
