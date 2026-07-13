/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 10:34:14 by jotriano          #+#    #+#             */
/*   Updated: 2026/07/08 10:34:14 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	abort_simulation(t_data *data)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->stop_simulation = 1;
	pthread_mutex_unlock(&data->stop_mutex);
	join_and_clean(data);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	d;

	if (argc != 9)
	{
		printf("Error: Invalid arguments.\n");
		return (1);
	}
	if (init_args(&d, argv))
	{
		printf("Error: Invalid argument values or scheduler.\n");
		return (1);
	}
	if (init_sim_data(&d))
	{
		free_all_data(&d);
		return (1);
	}
	if (start_simulation(&d))
		return (abort_simulation(&d));
	join_and_clean(&d);
	return (0);
}
