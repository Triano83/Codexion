/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-26 10:05:33 by jotriano          #+#    #+#             */
/*   Updated: 2026-06-26 10:05:33 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	coder = (t_coder *)arg;
	printf("Coder %d: ¡He nacido y estoy listo para tirar código! 💻\n", coder->id);
	return (NULL);
}

int	start_simulation(t_data *data)
{
	int	i;
	i = 0;
	while (i < data->num_coders)
	{
		data->coders[i].last_compile_start = get_time_in_ms();
		if (pthread_create(&data->coders[i].thread, NULL, &coder_routine, &data->coders[i]) != 0)
			return (printf("Error: No se pudo crear el hilo del coder %d\n", i + 1), 0);
		i++;
	}
	i = 0;
	while (i < data->num_coders)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
	return (1);
}
