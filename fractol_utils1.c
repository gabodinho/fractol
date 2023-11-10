/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiertzu <ggiertzu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 17:33:00 by ggiertzu          #+#    #+#             */
/*   Updated: 2023/11/08 12:16:47 by ggiertzu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <unistd.h>
#include <stdio.h>

void	comp_sq(double *comp)
{
	double	real;
	double	img;

	if (!comp)
		return;
	real = comp[0];
	img = comp[1];
	comp[0] = pow(real, 2) - pow(img, 2);
	comp[1] = real * img;
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
	res = sqrt(pow(comp[0], 2) + pow(comp[1], 2));
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

/* idea to take a function as an argument to work with other sets. Prototype looks like this:
int	steps(void *(*set)(double *, double *),double *point, int max, double thresh)
*/

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
//	if (MAXRUNS < 1 || THRESH < 0)
//		return 0;
	while (i < MAXRUNS && res < THRESH)
	{
		res = mandel_step(prev, point);
		printf("%f\n", res);
		i++;
	}
	return i;
}


int	main(void)
{
	double a[2] = {3, 4};
	double b[2] = {5, 7};
	double c[2] = {0.12, 0.24};
	printc(a);
	comp_sq(a);
	printc(a);
	comp_sum(b, c);
	printc(b);
	printf("abs: %f\n", comp_abs(b));
	printf("result for point c: %d\n", steps_mandel(c, 20, 100));
	return 0;
}

