/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_sync.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:56:50 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 21:38:50 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
