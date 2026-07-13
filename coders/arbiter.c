/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arbiter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 10:30:47 by jotriano          #+#    #+#             */
/*   Updated: 2026/07/08 10:30:47 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	insert_request_safe(t_coder *coder, t_request *req)
{
	t_data	*d;

	d = coder->data;
	req->coder_id = coder->id;
	if (d->scheduler == FIFO)
		req->priority = get_time_ms();
	else
		req->priority = coder->last_compile_start + d->time_to_burnout;
	insert_heap(d, *req);
}

static int	priority_competitor(t_coder *c, t_request *req)
{
	int			i;
	int			o_l;
	int			o_r;
	long long	n;
	t_request	r;

	i = -1;
	n = get_time_ms();
	while (++i < c->data->request_heap.size)
	{
		r = c->data->request_heap.array[i];
		if (r.coder_id == c->id || !is_higher_priority(r, *req))
			continue ;
		o_l = r.coder_id - 1;
		o_r = r.coder_id % c->data->nb_coders;
		if (c->data->dongle_state[o_l] != 0 || c->data->dongle_state[o_r] != 0)
			continue ;
		if (n < c->data->dongle_ready_time[o_l])
			continue ;
		if (n < c->data->dongle_ready_time[o_r])
			continue ;
		return (1);
	}
	return (0);
}

static int	try_take_dongles(t_coder *coder, t_request *req)
{
	int			l;
	int			r;
	long long	now;

	l = coder->id - 1;
	r = coder->id % coder->data->nb_coders;
	if (priority_competitor(coder, req))
		return (0);
	if (coder->data->dongle_state[l] != 0 || coder->data->dongle_state[r] != 0)
		return (0);
	now = get_time_ms();
	if (now < coder->data->dongle_ready_time[l]
		|| now < coder->data->dongle_ready_time[r])
		return (2);
	extract_node(coder->data, coder->id);
	lock_dongles(coder->data, l, r);
	coder->data->dongle_state[l] = 1;
	coder->data->dongle_state[r] = 1;
	return (1);
}

static int	attempt_loop(t_coder *coder, t_request *req)
{
	int		st;
	t_data	*d;

	d = coder->data;
	while (!check_stop(d))
	{
		st = try_take_dongles(coder, req);
		if (st == 1)
			break ;
		if (st == 2)
		{
			pthread_mutex_unlock(&d->arbiter_mutex);
			usleep(1000);
			pthread_mutex_lock(&d->arbiter_mutex);
		}
		else
			pthread_cond_wait(&d->arbiter_cond, &d->arbiter_mutex);
	}
	return (st);
}

int	take_dongles(t_coder *coder)
{
	t_data		*d;
	t_request	req;
	int			st;

	d = coder->data;
	pthread_mutex_lock(&d->arbiter_mutex);
	insert_request_safe(coder, &req);
	st = 0;
	if (!check_stop(d))
		st = attempt_loop(coder, &req);
	if (st == 1)
	{
		coder->last_compile_start = get_time_ms();
		pthread_cond_broadcast(&d->arbiter_cond);
	}
	pthread_mutex_unlock(&d->arbiter_mutex);
	return (st == 1);
}
