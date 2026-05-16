/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_sync.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:56:50 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 22:17:39 by jmanani          ###   ########.fr       */
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
	mutex_safe(&dongle->dongle_state_mutex, LOCK);
	while (!coding_finished(cd)
		&& get_time(MILLISEC) < dongle->next_available_t)
	{
		wait_msec = dongle->next_available_t - get_time(MILLISEC);
		abs_usec = get_time(MICROSEC) + (wait_msec * 1e3);
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
	mutex_safe(&dongle->dongle_mutex, LOCK);
	mutex_safe(&dongle->dongle_state_mutex, UNLOCK);
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
