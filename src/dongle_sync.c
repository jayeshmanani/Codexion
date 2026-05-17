/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_sync.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:56:50 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/17 19:32:19 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	dongle_wait_cond(t_dongle *dongle)
{
	long			wait_msec;
	long			abs_usec;
	struct timespec	time_spec;
	int				rc;

	wait_msec = dongle->next_available_t - get_time(MILLISEC);
	if (wait_msec <= 0)
		return ;
	abs_usec = get_time(MICROSEC) + (wait_msec * 1e3);
	abs_time_from_usec(abs_usec, &time_spec);
	rc = pthread_cond_timedwait(&dongle->dongle_cond,
			&dongle->dongle_state_mutex, &time_spec);
	if (rc != 0 && rc != ETIMEDOUT)
		err_and_exit("Error: cond_timedwait failed in dongle_acquire\n");
	return ;
}

static int	try_acquire(t_coder *coder, t_dongle *dongle)
{
	if (heap_peek(dongle->access_heap).coder_id == coder->coder_id)
	{
		if (dongle->is_taken == false)
		{
			if (get_time(MILLISEC) >= dongle->next_available_t)
			{
				dongle->is_taken = true;
				heap_pop(dongle->access_heap);
				return (1);
			}
			dongle_wait_cond(dongle);
		}
		else
			cond_safe(&dongle->dongle_cond, &dongle->dongle_state_mutex, WAIT,
				NULL);
	}
	else
		cond_safe(&dongle->dongle_cond, &dongle->dongle_state_mutex, WAIT,
			NULL);
	return (0);
}

void	acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	t_req	req;

	if (!coder || !dongle)
		err_and_exit("acquire_dongle: null arg");
	req.coder_id = coder->coder_id;
	req.arrival_t = get_time(MILLISEC);
	req.deadline_t = get_long(&coder->coder_mutex, &coder->last_compile_t)
		+ coder->cd->burn_time;
	mutex_safe(&dongle->dongle_state_mutex, LOCK);
	heap_push(dongle->access_heap, req);
	while (!coding_finished(coder->cd))
	{
		if (try_acquire(coder, dongle))
		{
			mutex_safe(&dongle->dongle_state_mutex, UNLOCK);
			return ;
		}
	}
	mutex_safe(&dongle->dongle_state_mutex, UNLOCK);
}

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	if (!coder || !dongle)
		err_and_exit("release_dongle: null arg");
	mutex_safe(&dongle->dongle_state_mutex, LOCK);
	dongle->is_taken = false;
	dongle->next_available_t = get_time(MILLISEC) + coder->cd->cooldown_time;
	if (cond_safe(&dongle->dongle_cond, NULL, BROADCAST, NULL) != 0)
		err_and_exit("Error: cond_safe failed in release_dongle\n");
	mutex_safe(&dongle->dongle_state_mutex, UNLOCK);
}
