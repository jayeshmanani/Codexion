/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 23:35:09 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 12:57:47 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	waiting_for_coders(t_coding_data *cd)
{
	while ((get_bool(&cd->cd_mutex, &cd->coders_ready)) == false)
		usleep(100);
}

void	increase_long(t_mtx *mutex, long *val)
{
	mutex_safe(mutex, LOCK);
	(*val)++;
	mutex_safe(mutex, UNLOCK);
}

bool	all_coders_ready(t_mtx *mutex, long *threads, long coder_nbr)
{
	bool	ready;

	mutex_safe(mutex, LOCK);
	ready = (*threads == coder_nbr);
	mutex_safe(mutex, UNLOCK);
	return (ready);
}
