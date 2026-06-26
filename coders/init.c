/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-25 09:28:42 by jotriano          #+#    #+#             */
/*   Updated: 2026-06-25 09:28:42 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void enter_data(t_data *data, char **argv)
{
	data->num_coders = ft_atoi(argv[1]);
	data->time_to_burnout = ft_atoi(argv[2]);
	data->time_to_compile = ft_atoi(argv[3]);
	data->time_to_debug = ft_atoi(argv[4]);
	data->time_to_refactor = ft_atoi(argv[5]);
	data->num_compiles_req = ft_atoi(argv[6]);
	data->dongle_cooldown = ft_atoi(argv[7]);
	data->is_edf = (strcmp(argv[8], "edf") == 0) ? 1 : 0;
}
int	allocate_resources(t_data *data)
{
	data->coders = malloc(sizeof(t_coder) * data->num_coders);
	data->dongles = malloc(sizeof(t_dongle) * data->num_coders);
	if (!data->coders || !data->dongles)
		return (free(data->coders),free(data->dongles),0);
	return (1);
}
int	init_dongles(t_data *data)
{
	int i;

	i = 0;
	while (i < data->num_coders)
	{
		data->dongles[i].id = i + 1;
		data->dongles[i].is_taken = 0;
		data->dongles[i].available_at = 0;
		data->dongles[i].heap_size = 0;
		data->dongles[i].heap = malloc(sizeof(t_heap_node) * data->num_coders);
		if (!data->dongles[i].heap)
			while (i > 0)
				free(data->dongles[i--].heap);
			return(0);
	}
}

int	init_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].state = 0;
		data->coders[i].compiles_count = 0;
		data->coders[i].last_compile_start = 0;
		data->coders[i].data = data;
		data->coders[i].left_dongle = i;
		data->coders[i].right_dongle = (i + 1) % data->num_coders;
		i++;
	}
	return (1);
}
int	init_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].state = 0;
		data->coders[i].compiles_count = 0;
		data->coders[i].last_compile_start = 0;
		data->coders[i].data = data;
		data->coders[i].left_dongle = i;
		data->coders[i].right_dongle = (i + 1) % data->num_coders;
		i++;
	}
	return (1);
}
