/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_sync.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:56:50 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/17 15:12:47 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// static void	dongle_acquire_utils(t_dongle *dongle)
// {
// 	long			wait_msec;
// 	long			abs_usec;
// 	struct timespec	time_spec;
// 	int rc;

// 	wait_msec = dongle->next_available_t - get_time(MILLISEC);
// 	abs_usec = get_time(MICROSEC) + (wait_msec * 1e3);
// 	abs_time_from_usec(abs_usec, &time_spec);
// 	rc = pthread_cond_timedwait(&dongle->dongle_cond,
// 			&dongle->dongle_state_mutex, &time_spec);
// 	if (rc != 0 && rc != ETIMEDOUT)
// 		err_and_exit("Error: cond_timedwait failed in dongle_acquire\n");
// 	return ;
// }

void	acquire_dongle(t_coding_data *cd, t_dongle *dongle)
{
	long			wait_msec;
	long			abs_usec;
	struct timespec	time_spec;

	if (!cd || !dongle)
		err_and_exit("acquire_dongle: null arg");
	/*
		* Avoid lock-order inversion between dongle_state_mutex and dongle_mutex
		* by never holding both at the same time while attempting acquisition.
		*/
	while (!coding_finished(cd))
	{
		mutex_safe(&dongle->dongle_state_mutex, LOCK);
		while (!coding_finished(cd)
			&& get_time(MILLISEC) < dongle->next_available_t)
		{
			wait_msec = dongle->next_available_t - get_time(MILLISEC);
			abs_usec = get_time(MICROSEC) + (wait_msec * 1000);
			abs_time_from_usec(abs_usec, &time_spec);
			if (cond_safe(&dongle->dongle_cond, &dongle->dongle_state_mutex,
					TIMEDWAIT, &time_spec) != 0)
				err_and_exit("Error: cond_safe failed in acquire_dongle\n");
		}
		if (coding_finished(cd))
		{
			mutex_safe(&dongle->dongle_state_mutex, UNLOCK);
			return ;
		}
		/* try to take the dongle without holding the state mutex */
		mutex_safe(&dongle->dongle_state_mutex, UNLOCK);
		mutex_safe(&dongle->dongle_mutex, LOCK);
		/* re-check under state mutex that the dongle is still available */
		mutex_safe(&dongle->dongle_state_mutex, LOCK);
		if (get_time(MILLISEC) >= dongle->next_available_t)
		{
			mutex_safe(&dongle->dongle_state_mutex, UNLOCK);
			return ;
		}
		/* not available anymore, release and retry */
		mutex_safe(&dongle->dongle_state_mutex, UNLOCK);
		mutex_safe(&dongle->dongle_mutex, UNLOCK);
		usleep(100);
	}
}

void	release_dongle(t_coding_data *cd, t_dongle *dongle)
{
	if (!cd || !dongle)
		err_and_exit("release_dongle: null arg");
	mutex_safe(&dongle->dongle_state_mutex, LOCK);
	dongle->next_available_t = get_time(MILLISEC) + cd->cooldown_time;
	if (cond_safe(&dongle->dongle_cond, NULL, BROADCAST, NULL) != 0)
		err_and_exit("Error: cond_safe failed in release_dongle\n");
	mutex_safe(&dongle->dongle_state_mutex, UNLOCK);
	mutex_safe(&dongle->dongle_mutex, UNLOCK);
}
