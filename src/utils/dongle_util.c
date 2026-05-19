/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:37:54 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/19 08:59:01 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	destroy_dongle(t_dongle *dongle)
{
	if (!dongle)
		return (1);
	if (dongle->dongle_mutex_initialized)
	{
		if (mutex_safe(&dongle->dongle_mutex, DESTROY) != 0)
			return (1);
		dongle->dongle_mutex_initialized = false;
	}
	if (dongle->dongle_cond_initialized)
	{
		if (cond_safe(&dongle->dongle_cond, NULL, DESTROY, NULL) != 0)
			return (1);
		dongle->dongle_cond_initialized = false;
	}
	heap_destroy(dongle->access_heap);
	dongle->access_heap = NULL;
	return (0);
}

int	destroy_all_dongles(t_coding_data *cd)
{
	int			i;
	t_dongle	*dongle;

	i = -1;
	if (!cd || !cd->dongles)
		return (1);
	while (++i < cd->n_coders)
	{
		dongle = &cd->dongles[i];
		if (dongle)
			if (destroy_dongle(dongle))
				return (1);
	}
	free(cd->dongles);
	cd->dongles = NULL;
	return (0);
}

int	init_dongle(t_dongle *dongle, t_coding_data *cd)
{
	if (!dongle || !cd)
		return (1);
	dongle->dongle_cond_initialized = true;
	dongle->next_available_t = 0;
	dongle->is_taken = false;
	dongle->access_heap = malloc(sizeof(t_heap));
	if (!dongle->access_heap)
		return (1);
	dongle->access_heap->arr = malloc(cd->n_coders * sizeof(t_req));
	if (!dongle->access_heap->arr)
	{
		free(dongle->access_heap);
		dongle->access_heap = NULL;
		return (1);
	}
	dongle->access_heap->size = 0;
	dongle->access_heap->capacity = cd->n_coders;
	dongle->access_heap->scheduler = cd->scheduler;
	return (0);
}
