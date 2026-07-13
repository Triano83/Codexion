/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 10:34:22 by jotriano          #+#    #+#             */
/*   Updated: 2026/07/08 10:34:22 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_stop(t_data *data)
{
	int	stop;

	stop = 0;
	pthread_mutex_lock(&data->stop_mutex);
	stop = data->stop_simulation;
	pthread_mutex_unlock(&data->stop_mutex);
	return (stop);
}

void	print_action(t_coder *coder, char *action)
{
	long long	time;

	pthread_mutex_lock(&coder->data->write_mutex);
	if (!check_stop(coder->data))
	{
		time = get_time_ms() - coder->data->start_time;
		printf("%lld %d %s\n", time, coder->id, action);
	}
	pthread_mutex_unlock(&coder->data->write_mutex);
}

static void	run_cycle(t_coder *coder)
{
	print_action(coder, "has taken a dongle");
	print_action(coder, "has taken a dongle");
	print_action(coder, "is compiling");
	ft_usleep(coder->data->time_to_compile, coder->data);
	drop_dongles(coder);
	print_action(coder, "is debugging");
	ft_usleep(coder->data->time_to_debug, coder->data);
	print_action(coder, "is refactoring");
	ft_usleep(coder->data->time_to_refactor, coder->data);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->data->nb_coders == 1)
	{
		print_action(coder, "has taken a dongle");
		ft_usleep(coder->data->time_to_burnout + 10, coder->data);
		return (NULL);
	}
	while (!check_stop(coder->data))
	{
		pthread_mutex_lock(&coder->data->arbiter_mutex);
		if (coder->compiles_count >= coder->data->nb_compiles_required)
		{
			pthread_mutex_unlock(&coder->data->arbiter_mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->data->arbiter_mutex);
		if (take_dongles(coder) == 1)
			run_cycle(coder);
	}
	return (NULL);
}
