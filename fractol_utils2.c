/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiertzu <ggiertzu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 17:26:39 by ggiertzu          #+#    #+#             */
/*   Updated: 2023/11/22 18:46:29 by ggiertzu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// black to red
static int	cmap_def(int steps)
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

static int	cmap_advanced(int steps)
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

static	int	get_colour(int steps, int cmap)
{
	if (cmap == 1)
		return (cmap_def(steps));
	else if (cmap == 2)
		return (cmap_advanced(steps));
	return (-1);
}

static	void	put_colour(uint8_t *pixels, int rgba)
{
	if (!pixels)
		return ;
	*pixels = (rgba >> 24) & 0xFF;
	*(pixels + 1) = (rgba >> 16) & 0xFF;
	*(pixels + 2) = (rgba >> 8) & 0xFF;
	*(pixels + 3) = (rgba) & 0xFF;
	return ;
}

void	draw_image(mlx_image_t *img, t_limits lim)
{
	int	i;
	int	j;
	int	pixel_idx;
	int	colour;

	i = 0;
	pixel_idx = 0;
	colour = 0;
	while (i < WIDTH)
	{
		j = 0;
		while (j < HEIGHT)
		{
			pixel_idx = (j * WIDTH * 4) + (i * 4);
			colour = get_colour(get_steps(i, j, lim), lim.cmap);
			put_colour((img -> pixels) + pixel_idx, colour);
			j++;
		}
		i++;
	}
}
