/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiertzu <ggiertzu@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:57:54 by ggiertzu          #+#    #+#             */
/*   Updated: 2023/11/22 19:39:31 by ggiertzu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int32_t	main(int32_t argc, const char *argv[])
{
	mlx_t			*mlx;
	mlx_image_t		*image;
	t_limits		lim;
	t_input			input;

	input = check_input(argc, argv);
	mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true);
	image = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!mlx || !image)
		put_error(3);
	lim = init_lim(mlx, image, input);
	draw_image(image, lim);
	mlx_image_to_window(mlx, image, 0, 0);
	mlx_loop_hook(mlx, move_view, &lim);
	mlx_scroll_hook(mlx, zoom_hook, &lim);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
