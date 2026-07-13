/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 10:33:57 by jotriano          #+#    #+#             */
/*   Updated: 2026/07/08 10:33:57 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_heap(t_heap *heap, int capacity)
{
	heap->array = malloc(sizeof(t_request) * capacity);
	if (!heap->array)
		return (1);
	heap->size = 0;
	heap->capacity = capacity;
	return (0);
}

static void	heapify_up(t_data *data, int i)
{
	int			parent;
	t_request	temp;
	t_heap		*h;

	h = &data->request_heap;
	while (i != 0)
	{
		parent = (i - 1) / 2;
		if (!is_higher_priority(h->array[i], h->array[parent]))
			break ;
		temp = h->array[i];
		h->array[i] = h->array[parent];
		h->array[parent] = temp;
		i = parent;
	}
}

static void	heapify_down(t_data *data, int i)
{
	int			smallest;
	int			left;
	int			right;
	t_request	temp;
	t_request	*a;

	smallest = i;
	left = 2 * i + 1;
	right = 2 * i + 2;
	a = data->request_heap.array;
	if (left < data->request_heap.size
		&& is_higher_priority(a[left], a[smallest]))
		smallest = left;
	if (right < data->request_heap.size
		&& is_higher_priority(a[right], a[smallest]))
		smallest = right;
	if (smallest != i)
	{
		temp = a[i];
		a[i] = a[smallest];
		a[smallest] = temp;
		heapify_down(data, smallest);
	}
}

void	insert_heap(t_data *data, t_request req)
{
	int	i;

	i = data->request_heap.size;
	data->request_heap.array[i] = req;
	data->request_heap.size++;
	heapify_up(data, i);
}

void	extract_node(t_data *data, int coder_id)
{
	int		i;
	t_heap	*h;

	h = &data->request_heap;
	i = 0;
	while (i < h->size)
	{
		if (h->array[i].coder_id == coder_id)
			break ;
		i++;
	}
	if (i == h->size)
		return ;
	h->array[i] = h->array[h->size - 1];
	h->size--;
	if (i < h->size)
	{
		heapify_up(data, i);
		heapify_down(data, i);
	}
}
