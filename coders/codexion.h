/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-25 09:10:25 by jotriano          #+#    #+#             */
/*   Updated: 2026-06-25 09:10:25 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_heap_node
{
	int			coder_id;
	long long	priority;
}	t_heap_node;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				is_taken;
	long long		available_at;
	t_heap_node		*heap;
	int				heap_size;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	pthread_t	thread;
	int			state;
	int			left_dongle;
	int			right_dongle;
	long long	last_compile_start;
	int			compiles_count;
	struct s_data	*data;
}	t_coder;

typedef struct s_data
{
	int				num_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int				num_compiles_req;
	long long		dongle_cooldown;
	int				is_edf;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_data;

int	is_numeric_str(char *str);
int	ft_atoi(const char *nptr);
void enter_data(t_data *data, char **argv);
int	allocate_resources(t_data *data);
int	init_dongles(t_data *data);
int	init_coders(t_data *data);
long long	get_time_in_ms(void);

#endif