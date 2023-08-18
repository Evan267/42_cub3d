/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberger <eberger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 09:42:53 by eberger           #+#    #+#             */
/*   Updated: 2023/08/06 14:00:27 by eberger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	move(t_vars *vars, int direction)
{
	t_pers *pers = vars->pers;
	double	angle_rad;
	int		test[2];
	
	angle_rad = pers->orientation * M_PI / 180.0;
	if (direction == 1)
		angle_rad = (pers->orientation + 180) * M_PI / 180.0;
	else if (direction == 2)
		angle_rad = (pers->orientation + 90) * M_PI / 180.0;
	else if (direction == 3)
		angle_rad = (pers->orientation + 270) * M_PI / 180.0;
	double dx = (pers->ray_length * 2) * cos(angle_rad);
	double dy = (pers->ray_length * 2) * sin(angle_rad);

	int steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);

	double xIncrement = dx / steps;
	double yIncrement = dy / steps;

	double x1 = pers->x;
	double y1 = pers->y;
	test[0] = xIncrement < 0 ?
		floor(xIncrement) - 1 + pers->x:
		ceil(xIncrement) + 1 + pers->x;
	test[1] = yIncrement < 0 ?
		floor(yIncrement) - 1 + pers->y:
		ceil(yIncrement) + 1 + pers->y;
	if (find_case(vars->map->lines, test[0] / DIMENSION, test[1] / DIMENSION) != '1')
	{
		x1 += xIncrement * 100 * vars->mlx->delta_time;
		y1 += yIncrement * 100 * vars->mlx->delta_time;
		pers->x = round(x1);
		pers->y = round(y1);
	}
}

void	ft_hook_move(void* param)
{
	t_vars *vars;

	vars = param;
	if (mlx_is_key_down(vars->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(vars->mlx);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_UP) || mlx_is_key_down(vars->mlx, MLX_KEY_W))
		move(vars, 0);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_DOWN) || mlx_is_key_down(vars->mlx, MLX_KEY_S))
		move(vars, 1);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_A))
		move(vars, 2);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_D))
		move(vars, 3);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_LEFT))
		vars->pers->orientation -= 5 * vars->mlx->delta_time * 10;
	if (mlx_is_key_down(vars->mlx, MLX_KEY_RIGHT))
		vars->pers->orientation += 5 * vars->mlx->delta_time * 10;
}
