/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:55:00 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 13:55:00 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	has_ready_higher_coder(t_coder *coder, t_req req, long now)
{
	t_coder	*c;
	long	left_ready_t;
	long	right_ready_t;

	if (req.coder_id == coder->coder_id || find_preference(coder->coder_req,
			req, coder->cd->global_heap->scheduler))
		return (false);
	c = &coder->cd->coders[req.coder_id - 1];
	if (lock_dongles_pair(c->left_dongle, c->right_dongle) != 0)
		return (false);
	if (c->left_dongle->is_taken || c->right_dongle->is_taken)
	{
		unlock_dongles_pair(c->left_dongle, c->right_dongle);
		return (false);
	}
	left_ready_t = c->left_dongle->next_available_t;
	right_ready_t = c->right_dongle->next_available_t;
	unlock_dongles_pair(c->left_dongle, c->right_dongle);
	return (left_ready_t <= now && right_ready_t <= now);
}

bool	is_coder_highest_ready(t_coder *coder)
{
	long	i;
	t_heap	*heap;
	t_req	req;
	long	now;

	heap = coder->cd->global_heap;
	now = get_time(MILLISEC);
	i = -1;
	while (++i < heap->size)
	{
		req = heap->arr[i];
		if (has_ready_higher_coder(coder, req, now))
			return (false);
	}
	return (true);
}

int	init_global_queue(t_coding_data *cd)
{
	if (!cd)
		return (1);
	cd->global_heap = malloc(sizeof(t_heap));
	if (!cd->global_heap)
		return (1);
	cd->global_heap->arr = malloc(cd->n_coders * sizeof(t_req));
	if (!cd->global_heap->arr)
	{
		free(cd->global_heap);
		cd->global_heap = NULL;
		return (1);
	}
	cd->global_heap->size = 0;
	cd->global_heap->capacity = cd->n_coders;
	cd->global_heap->scheduler = cd->scheduler;
	if (mutex_safe(&cd->global_mutex, INIT) != 0)
		return (1);
	cd->global_mutex_initialized = true;
	if (cond_safe(&cd->global_cond, NULL, INIT, NULL) != 0)
		return (1);
	cd->global_cond_initialized = true;
	return (0);
}
