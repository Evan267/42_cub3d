/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberger <eberger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 11:43:55 by eberger           #+#    #+#             */
/*   Updated: 2023/08/06 14:39:26 by eberger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	find_length(int x, int y, t_pers *pers)
{
	int	a;
	int	b;
	int	c;

	a = pow(pers->x - x, 2);
	b = pow(pers->y - y, 2);
	c = sqrt(a + b);
	return (c);
}

int	init_points(int length, t_ray *ray)
{
	int	i;

	i = 0;
	if (ray->count_points < length)
	{
		if (ray->count_points != 0)
			free(ray->points);
		ray->points = malloc(sizeof(t_point) * length);
		if (!ray->points)
			return (1);
		ray->count_points = length;
		while (i < length)
		{
			ray->points[i].x = 0;
			ray->points[i].y = 0;
			ray->points[i].enabled = 0;
			i++;
		}
	}
	return (0);
}

int	calc_line(int x1, int y1, int id_ray, t_vars *vars)
{
	t_ray	**rays;
	t_pers	*pers;
	int		i;

	i = 0;
	pers = vars->pers;
	rays = pers->rays;
	double	angle_rad = rays[id_ray]->dir * M_PI / 180.0;
	double dx = pers->ray_length * cos(angle_rad);
	double dy = pers->ray_length * sin(angle_rad);

	int steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
	if (init_points(steps, rays[id_ray]))
		return (1);

	double xIncrement = dx / steps;
	double yIncrement = dy / steps;

	double x = x1;
	double y = y1;

	i = 0;
	while (i <= steps && find_case(vars->map->lines, x / DIMENSION, y / DIMENSION) != '1')
	{
		rays[id_ray]->points[i].x = round(x);
		rays[id_ray]->points[i].y = round(y);
		rays[id_ray]->points[i].enabled = 1;
		x += xIncrement * (i / 50 + 1);
		y += yIncrement * (i / 50 + 1);
		i++;
	}
	rays[id_ray]->length = find_length(round(x - xIncrement), round(y - yIncrement), pers);
	rays[id_ray]->col_x = round(x - xIncrement);
	rays[id_ray]->col_y = round(y - yIncrement);
	rays[id_ray]->distance_proj = rays[id_ray]->length * cos(rays[id_ray]->dir);
	while (i < rays[id_ray]->count_points)
	{
		rays[id_ray]->points[i].enabled = 0;
		i++;
	}
	return (0);
}


//A supprimer
int	draw_line(int x1, int y1, int id_ray, t_vars *vars)
{
	t_ray	**rays;
	t_pers	*pers;
	int		i;

	i = 0;
	pers = vars->pers;
	rays = pers->rays;
	double	angle_rad = rays[id_ray]->dir * M_PI / 180.0;
	double dx = pers->ray_length * cos(angle_rad);
	double dy = pers->ray_length * sin(angle_rad);

	int steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);

	double xIncrement = dx / steps;
	double yIncrement = dy / steps;

	double x = x1;
	double y = y1;

	uint32_t color = get_rgba(255, 0, 0, 255);
	i = 0;
	while (i <= steps && find_case(vars->map->lines, x / DIMENSION, y / DIMENSION) != '1')
	{
		if (rays[id_ray]->img->count < (unsigned long)steps)
		{
			if (mlx_image_to_window(vars->mlx, rays[id_ray]->img, round(x), round(y)) == -1)
				return (printf("error\n"), 1);
			if (put_pixels(rays[id_ray]->img, color))
				return (printf("error\n"), 1);
		}
		else
		{
			rays[id_ray]->img->instances[i].x = round(x);
			rays[id_ray]->img->instances[i].y = round(y);
			rays[id_ray]->img->instances[i].enabled = 1;
		}
		x += xIncrement;
		y += yIncrement;
		i++;
	}
	while (i < (int)rays[id_ray]->img->count)
	{
		rays[id_ray]->img->instances[i].enabled = 0;
		i++;
	}
	return (0);
}

void	ft_raycasting(void *param)
{
	t_vars	*vars;
	t_pers	*pers;
	int		i;

	vars = param;
	pers = vars->pers;
	i = 0;
	while (pers->rays[i])
	{
		pers->rays[i]->dir = pers->orientation + (i * 60 / 1080) - 30;
		if (MAP == 1)
			draw_line(pers->x, pers->y, i, vars);
		calc_line(pers->x, pers->y, i, vars);
		i++;
	}
}
