/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:37:54 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/17 14:45:21 by jmanani          ###   ########.fr       */
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
	if (dongle->dongle_state_mutex_initialized)
	{
		if (mutex_safe(&dongle->dongle_state_mutex, DESTROY) != 0)
			return (1);
		dongle->dongle_state_mutex_initialized = false;
	}
	if (dongle->dongle_cond_initialized)
	{
		if (cond_safe(&dongle->dongle_cond, NULL, DESTROY, NULL) != 0)
			return (1);
		dongle->dongle_cond_initialized = false;
	}
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
