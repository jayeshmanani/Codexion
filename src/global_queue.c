/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_queue.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 12:23:37 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 18:05:03 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	is_coder_highest_ready(t_coder *coder)
{
	long	i;
	t_heap	*heap;
	t_req	req;
	t_coder	*c;
	long	now;

	heap = coder->cd->global_heap;
	now = get_time(MILLISEC);
	i = -1;
	while (++i < heap->size)
	{
		req = heap->arr[i];
		if (req.coder_id == coder->coder_id)
			continue ;
		if (find_preference(coder->coder_req, req, heap->scheduler))
			continue ;
		c = &coder->cd->coders[req.coder_id - 1];
		if (c->left_dongle->is_taken || c->right_dongle->is_taken)
			continue ;
		if (c->left_dongle->next_available_t <= now
			&& c->right_dongle->next_available_t <= now)
			return (false);
	}
	return (true);
}

static int	global_turn_wait(t_coder *coder, t_req *top_req)
{
	if (heap_peek(coder->cd->global_heap, top_req) != 0)
	{
		if (cond_safe(&coder->cd->global_cond, &coder->cd->global_mutex, WAIT,
				NULL) != 0)
			return (-1);
		return (0);
	}
	if (!is_coder_highest_ready(coder))
	{
		if (cond_safe(&coder->cd->global_cond, &coder->cd->global_mutex, WAIT,
				NULL) != 0)
			return (-1);
		return (0);
	}
	return (1);
}

static int	global_wait_time(t_coding_data *cd, long wait_msec)
{
	if (wait_msec <= 0)
		return (cond_safe(&cd->global_cond, &cd->global_mutex, WAIT, NULL));
	mutex_safe(&cd->global_mutex, UNLOCK);
	usleep(wait_msec * 1000);
	mutex_safe(&cd->global_mutex, LOCK);
	return (0);
}

static int	global_take_both(t_coder *coder, t_req *top_req, long *wait_msec)
{
	(void)top_req;
	if (take_both_if_ready(coder, wait_msec) != 0)
		return (-1);
	if (*wait_msec != 0)
		return (1);
	if (heap_remove(coder->cd->global_heap, coder->coder_id) != 0)
	{
		reset_dongles_pair(coder->left_dongle, coder->right_dongle);
		return (-1);
	}
	cond_safe(&coder->cd->global_cond, NULL, BROADCAST, NULL);
	mutex_safe(&coder->cd->global_mutex, UNLOCK);
	return (0);
}

int	register_global_request(t_coder *coder, t_req req)
{
	if (!coder || !coder->cd || !coder->cd->global_heap)
		return (1);
	if (mutex_safe(&coder->cd->global_mutex, LOCK) != 0)
		return (1);
	if (heap_push(coder->cd->global_heap, req) != 0)
	{
		mutex_safe(&coder->cd->global_mutex, UNLOCK);
		return (1);
	}
	cond_safe(&coder->cd->global_cond, NULL, BROADCAST, NULL);
	if (mutex_safe(&coder->cd->global_mutex, UNLOCK) != 0)
		return (1);
	return (0);
}
