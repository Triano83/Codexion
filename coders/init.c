/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 10:34:07 by jotriano          #+#    #+#             */
/*   Updated: 2026/07/08 10:34:07 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_args(t_data *data, char **argv)
{
	memset(data, 0, sizeof(t_data));
	data->nb_coders = (int)ft_atoll(argv[1]);
	data->time_to_burnout = ft_atoll(argv[2]);
	data->time_to_compile = ft_atoll(argv[3]);
	data->time_to_debug = ft_atoll(argv[4]);
	data->time_to_refactor = ft_atoll(argv[5]);
	data->nb_compiles_required = (int)ft_atoll(argv[6]);
	data->dongle_cooldown = ft_atoll(argv[7]);
	if (data->nb_coders <= 0 || data->time_to_burnout < 0)
		return (1);
	if (strcmp(argv[8], "fifo") == 0)
		data->scheduler = FIFO;
	else if (strcmp(argv[8], "edf") == 0)
		data->scheduler = EDF;
	else
		return (1);
	return (0);
}

static void	init_mutexes(t_data *data)
{
	int	i;

	pthread_mutex_init(&data->write_mutex, NULL);
	pthread_mutex_init(&data->stop_mutex, NULL);
	pthread_mutex_init(&data->arbiter_mutex, NULL);
	pthread_cond_init(&data->arbiter_cond, NULL);
	i = -1;
	while (++i < data->nb_coders)
		pthread_mutex_init(&data->dongle_mutex[i], NULL);
}

int	init_sim_data(t_data *data)
{
	data->dongle_state = malloc(sizeof(int) * data->nb_coders);
	if (!data->dongle_state)
		return (1);
	data->dongle_ready_time = malloc(sizeof(long long) * data->nb_coders);
	if (!data->dongle_ready_time)
		return (1);
	data->dongle_mutex = malloc(sizeof(pthread_mutex_t) * data->nb_coders);
	if (!data->dongle_mutex)
		return (1);
	data->coders = malloc(sizeof(t_coder) * data->nb_coders);
	if (!data->coders)
		return (1);
	if (init_heap(&data->request_heap, data->nb_coders))
		return (1);
	init_mutexes(data);
	memset(data->dongle_state, 0, sizeof(int) * data->nb_coders);
	memset(data->dongle_ready_time, 0, sizeof(long long) * data->nb_coders);
	return (0);
}

int	start_simulation(t_data *data)
{
	int			i;
	pthread_t	*thread;
	t_coder		*coder;

	data->start_time = get_time_ms();
	if (data->start_time == -1)
		return (1);
	i = -1;
	while (++i < data->nb_coders)
	{
		thread = &data->coders[i].thread_id;
		coder = &data->coders[i];
		data->coders[i].id = i + 1;
		data->coders[i].last_compile_start = data->start_time;
		data->coders[i].compiles_count = 0;
		data->coders[i].data = data;
		if (pthread_create(thread, NULL, coder_routine, coder) != 0)
			return (1);
	}
	if (pthread_create(&data->monitor_thread, NULL, monitor_routine, data) != 0)
		return (1);
	return (0);
}
