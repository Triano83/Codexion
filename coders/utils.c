/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 10:34:28 by jotriano          #+#    #+#             */
/*   Updated: 2026/07/08 10:34:28 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long long	ft_atoll(const char *str)
{
	long long	res;
	int			i;

	res = 0;
	i = 0;
	if (!str || !str[i])
		return (-1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res);
}

void	ft_usleep(long long time_in_ms, t_data *data)
{
	long long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < time_in_ms)
	{
		pthread_mutex_lock(&data->stop_mutex);
		if (data->stop_simulation)
		{
			pthread_mutex_unlock(&data->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->stop_mutex);
		usleep(500);
	}
}

void	drop_dongles(t_coder *coder)
{
	t_data		*d;
	int			l;
	int			r;
	long long	now;

	d = coder->data;
	l = coder->id - 1;
	r = coder->id % d->nb_coders;
	unlock_dongles(d, l, r);
	pthread_mutex_lock(&coder->data->arbiter_mutex);
	coder->compiles_count++;
	now = get_time_ms();
	d->dongle_state[l] = 0;
	d->dongle_state[r] = 0;
	d->dongle_ready_time[l] = now + d->dongle_cooldown;
	d->dongle_ready_time[r] = now + d->dongle_cooldown;
	pthread_cond_broadcast(&coder->data->arbiter_cond);
	pthread_mutex_unlock(&coder->data->arbiter_mutex);
}

int	is_higher_priority(t_request a, t_request b)
{
	if (a.priority < b.priority)
		return (1);
	return (0);
}
