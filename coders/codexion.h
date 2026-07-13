/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 10:32:15 by jotriano          #+#    #+#             */
/*   Updated: 2026/07/08 10:32:15 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_request
{
	int			coder_id;
	long long	priority;
}	t_request;

typedef struct s_heap
{
	t_request	*array;
	int			size;
	int			capacity;
}	t_heap;

struct	s_data;

typedef struct s_coder
{
	int				id;
	pthread_t		thread_id;
	long long		last_compile_start;
	int				compiles_count;
	struct s_data	*data;
}	t_coder;

typedef struct s_data
{
	int				nb_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int				nb_compiles_required;
	long long		dongle_cooldown;
	t_scheduler		scheduler;

	long long		start_time;
	int				stop_simulation;

	pthread_mutex_t	write_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	arbiter_mutex;
	pthread_cond_t	arbiter_cond;
	pthread_mutex_t	*dongle_mutex;
	pthread_t		monitor_thread;

	int				*dongle_state;
	long long		*dongle_ready_time;

	t_heap			request_heap;
	t_coder			*coders;
}	t_data;

long long	ft_atoll(const char *str);
int			init_args(t_data *data, char **argv);
int			init_heap(t_heap *heap, int capacity);
void		insert_heap(t_data *data, t_request req);
int			is_higher_priority(t_request a, t_request b);
int			check_all_compiled(t_data *data);
void		extract_node(t_data *data, int coder_id);
long long	get_time_ms(void);
void		ft_usleep(long long time_in_ms, t_data *data);
int			init_sim_data(t_data *data);
int			start_simulation(t_data *data);
void		*coder_routine(void *arg);
int			check_stop(t_data *data);
int			take_dongles(t_coder *coder);
void		drop_dongles(t_coder *coder);
void		*monitor_routine(void *arg);
void		free_all_data(t_data *data);
void		join_and_clean(t_data *data);
int			abort_simulation(t_data *data);
void		lock_dongles(t_data *d, int l, int r);
void		unlock_dongles(t_data *d, int l, int r);

#endif