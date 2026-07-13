/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 10:33:49 by jotriano          #+#    #+#             */
/*   Updated: 2026/07/08 10:33:49 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	lock_dongles(t_data *d, int l, int r)
{
	if (l < r)
	{
		pthread_mutex_lock(&d->dongle_mutex[l]);
		pthread_mutex_lock(&d->dongle_mutex[r]);
	}
	else
	{
		pthread_mutex_lock(&d->dongle_mutex[r]);
		pthread_mutex_lock(&d->dongle_mutex[l]);
	}
}

void	unlock_dongles(t_data *d, int l, int r)
{
	if (l < r)
	{
		pthread_mutex_unlock(&d->dongle_mutex[r]);
		pthread_mutex_unlock(&d->dongle_mutex[l]);
	}
	else
	{
		pthread_mutex_unlock(&d->dongle_mutex[l]);
		pthread_mutex_unlock(&d->dongle_mutex[r]);
	}
}
