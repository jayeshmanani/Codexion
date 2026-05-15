/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arbiter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:54:34 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/15 19:11:32 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*arbiter_thread(void *args)
{
	t_coding_data	*cd;

	cd = (t_coding_data *)args;
	mutex_safe(&cd->cd_mutex, LOCK);
	while (!coding_finished(cd))
	{
		pthread_cond_wait(&cd->arbiter_cond, &cd->cd_mutex);
	}
	mutex_safe(&cd->cd_mutex, UNLOCK);
	return (NULL);
}
