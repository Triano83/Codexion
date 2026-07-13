/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 10:31:20 by jotriano          #+#    #+#             */
/*   Updated: 2026/07/08 10:31:20 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
free_all_data: Destroy the mutexes and free all the mallocs.
*/
void	free_all_data(t_data *data)
{
	int	i;

	if (data->dongle_mutex)
	{
		i = -1;
		while (++i < data->nb_coders)
			pthread_mutex_destroy(&data->dongle_mutex[i]);
		free(data->dongle_mutex);
	}
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	pthread_mutex_destroy(&data->arbiter_mutex);
	pthread_cond_destroy(&data->arbiter_cond);
	if (data->dongle_state)
		free(data->dongle_state);
	if (data->dongle_ready_time)
		free(data->dongle_ready_time);
	if (data->coders)
		free(data->coders);
	if (data->request_heap.array)
		free(data->request_heap.array);
}

/*
join_and_clean: Wait until the threads are finished and call for cleaning.
*/

void	join_and_clean(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_coders)
	{
		if (data->coders[i].thread_id)
			pthread_join(data->coders[i].thread_id, NULL);
	}
	if (data->monitor_thread)
		pthread_join(data->monitor_thread, NULL);
	free_all_data(data);
}
