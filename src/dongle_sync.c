/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_sync.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:56:50 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 13:23:41 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	dongle_wait_cond(t_dongle *dongle)
{
	long			wait_msec;
	long			abs_usec;
	struct timespec	time_spec;

	wait_msec = dongle->next_available_t - get_time(MILLISEC);
	if (wait_msec <= 0)
		return (0);
	abs_usec = get_time(MICROSEC) + (wait_msec * 1e3);
	abs_time_from_usec(abs_usec, &time_spec);
	if (cond_safe(&dongle->dongle_cond, &dongle->dongle_mutex, TIMEDWAIT,
			&time_spec) != 0)
		return (1);
	return (0);
}

static int	try_acquire(t_coder *coder, t_dongle *dongle)
{
	t_req	top_req;

	if (heap_peek(dongle->access_heap, &top_req) != 0)
		return (-1);
	if (top_req.coder_id != coder->coder_id || dongle->is_taken)
	{
		if (cond_safe(&dongle->dongle_cond, &dongle->dongle_mutex, WAIT,
				NULL) != 0)
			return (-1);
		return (0);
	}
	if (get_time(MILLISEC) >= dongle->next_available_t)
	{
		dongle->is_taken = true;
		if (heap_pop(dongle->access_heap, &top_req) != 0)
			return (-1);
		return (1);
	}
	if (dongle_wait_cond(dongle) != 0)
		return (-1);
	return (0);
}

int	wait_acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	int	status;

	if (!coder || !dongle)
		return (1);
	if (mutex_safe(&dongle->dongle_mutex, LOCK) != 0)
		return (1);
	status = 0;
	while (!coding_finished(coder->cd))
	{
		status = try_acquire(coder, dongle);
		if (status == -1 || status == 1)
			break ;
	}
	if (mutex_safe(&dongle->dongle_mutex, UNLOCK) != 0)
		return (1);
	if (status != -1)
		return (0);
	print_error("Error: wait_acquire_dongle failed");
	return (1);
}

int	acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	t_req	req;

	if (!coder || !dongle)
		return (1);
	req = coder->coder_req;
	req.deadline_t = get_long(&coder->coder_mutex, &coder->last_compile_t)
		+ coder->cd->burn_time;
	if (pre_register_dongle(coder, dongle, req) != 0)
		return (1);
	return (wait_acquire_dongle(coder, dongle));
}

int	release_dongle(t_coder *coder, t_dongle *dongle)
{
	if (!coder || !dongle)
		return (1);
	if (mutex_safe(&dongle->dongle_mutex, LOCK) != 0)
		return (1);
	dongle->is_taken = false;
	dongle->next_available_t = get_time(MILLISEC) + coder->cd->cooldown_time;
	if (cond_safe(&dongle->dongle_cond, NULL, BROADCAST, NULL) != 0)
	{
		mutex_safe(&dongle->dongle_mutex, UNLOCK);
		return (1);
	}
	if (mutex_safe(&dongle->dongle_mutex, UNLOCK) != 0)
		return (1);
	if (coder->cd->global_cond_initialized)
	{
		mutex_safe(&coder->cd->global_mutex, LOCK);
		cond_safe(&coder->cd->global_cond, NULL, BROADCAST, NULL);
		mutex_safe(&coder->cd->global_mutex, UNLOCK);
	}
	return (0);
}
