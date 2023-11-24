/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiertzu <ggiertzu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 17:23:50 by ggiertzu          #+#    #+#             */
/*   Updated: 2023/11/24 08:02:30 by ggiertzu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol.h>

static double	mandel_step(double *prev, double *c)
{
	double	temp;

	temp = prev[0] * prev[0] - prev[1] * prev[1] + c[0];
	prev[1] = prev[0] * prev[1] * 2 + c[1];
	prev[0] = temp;
	return (prev[0] * prev[0] + prev[1] * prev[1]);
}

static double	burning_step(double *prev, double *c)
{
	double	temp;

	temp = prev[0] * prev[0] - prev[1] * prev[1] + c[0];
	prev[1] = fabs(prev[0] * prev[1]) * 2 + c[1];
	prev[0] = temp;
	return (prev[0] * prev[0] + prev[1] * prev[1]);
}

static int	steps_mandel(double *point, t_f handler)
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
	return (i);
}

static int	steps_julia(double *point, double *c)
{
	int		i;
	double	res;

	i = 0;
	res = -1;
	while (i++ < MAXRUNS && res < THRESH)
		res = mandel_step(point, c);
	return (i);
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
