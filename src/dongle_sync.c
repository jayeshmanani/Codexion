/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_sync.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:56:50 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/15 15:23:16 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	dongle_acquire_utils(t_dongle *dongle, long *now)
{
	long			wait_usec;
	long			abs_usec;
	struct timespec	time_spec;
	int				ret;

	wait_usec = dongle->next_available_t - *now;
	abs_usec = get_time(MICROSEC) + wait_usec;
	abs_time_from_usec(abs_usec, &time_spec);
	if (pthread_cond_timedwait(&dongle->dongle_cond,
			&dongle->dongle_state_mutex, &time_spec) != 0)
		err_and_exit("Error: cond_timedwait failed in dongle_acquire\n");
}

void	acquire_dongle(t_coding_data *cd, t_dongle *dongle)
{
	long	now;
	int		ret;

	if (!cd || !dongle)
		err_and_exit("acquire_dongle: null arg");
	while (1)
	{
		mutex_safe(&dongle->dongle_state_mutex, LOCK);
		now = get_time(MICROSEC);
		if (now < dongle->next_available_t)
		{
			dongle_acquire_utils(dongle, &now);
			mutex_safe(&dongle->dongle_state_mutex, UNLOCK);
			continue ;
		}
		mutex_safe(&dongle->dongle_state_mutex, UNLOCK);
		mutex_safe(&dongle->dongle_mutex, LOCK);
		break ;
	}
}
