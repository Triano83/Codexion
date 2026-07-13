/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 10:34:19 by jotriano          #+#    #+#             */
/*   Updated: 2026/07/08 10:34:19 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
check_all_compiled: Check if everyone has reached the required number of builds.
*/
int	check_all_compiled(t_data *data)
{
	int	i;
	int	all_done;

	i = 0;
	all_done = 1;
	pthread_mutex_lock(&data->arbiter_mutex);
	while (i < data->nb_coders)
	{
		if (data->coders[i].compiles_count < data->nb_compiles_required)
		{
			all_done = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&data->arbiter_mutex);
	return (all_done);
}

static int	kill_coder(t_data *data, int id, long long now)
{
	pthread_mutex_lock(&data->write_mutex);
	if (!check_stop(data))
	{
		pthread_mutex_lock(&data->stop_mutex);
		data->stop_simulation = 1;
		pthread_mutex_unlock(&data->stop_mutex);
		pthread_cond_broadcast(&data->arbiter_cond);
		printf("%lld %d burned out\n", now - data->start_time, id);
	}
	pthread_mutex_unlock(&data->write_mutex);
	pthread_mutex_unlock(&data->arbiter_mutex);
	return (1);
}

/*
check_death: Check if someone has exceeded their time_to_burnout.
*/
static int	check_death(t_data *data)
{
	int			i;
	long long	now;
	t_coder		*c;

	i = -1;
	while (++i < data->nb_coders)
	{
		c = &data->coders[i];
		pthread_mutex_lock(&data->arbiter_mutex);
		if (c->compiles_count < data->nb_compiles_required)
		{
			now = get_time_ms();
			if (now - c->last_compile_start >= data->time_to_burnout)
			{
				return (kill_coder(data, c->id, now));
			}
		}
		pthread_mutex_unlock(&data->arbiter_mutex);
	}
	return (0);
}

/*
monitor_routine: The function that the supervisor thread executes.
*/
void	*monitor_routine(void *arg)
{
	t_data	*d;

	d = (t_data *)arg;
	while (!check_stop(d))
	{
		if (check_death(d) || check_all_compiled(d))
		{
			pthread_mutex_lock(&d->stop_mutex);
			d->stop_simulation = 1;
			pthread_mutex_unlock(&d->stop_mutex);
			pthread_mutex_lock(&d->arbiter_mutex);
			pthread_cond_broadcast(&d->arbiter_cond);
			pthread_mutex_unlock(&d->arbiter_mutex);
			break ;
		}
		usleep(500);
	}
	return (NULL);
}
