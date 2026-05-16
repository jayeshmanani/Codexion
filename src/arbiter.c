/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arbiter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:54:34 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 22:15:40 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*arbiter_thread(void *args)
{
	t_coding_data	*cd;
	t_req			req;
	t_coder			*coder;

	cd = (t_coding_data *)args;
	mutex_safe(&cd->cd_mutex, LOCK);
	while (!coding_finished(cd))
	{
		while (!coding_finished(cd) && heap_is_empty(cd))
			if (cond_safe(&cd->arbiter_cond, &cd->cd_mutex, WAIT, NULL) != 0)
				err_and_exit("Error: cond_safe failed in arbiter_thread\n");
		if (!coding_finished(cd))
		{
			req = heap_pop(cd->algo_heap);
			mutex_safe(&cd->cd_mutex, UNLOCK);
			coder = &cd->coders[req.coder_id - 1];
			mutex_safe(&coder->coder_mutex, LOCK);
			coder->req_pending = true;
			if (cond_safe(&coder->coder_req_cond, NULL, BROADCAST, NULL) != 0)
				err_and_exit("Error: cond_safe failed in arbiter_thread\n");
			mutex_safe(&coder->coder_mutex, UNLOCK);
			mutex_safe(&cd->cd_mutex, LOCK);
		}
	}
	mutex_safe(&cd->cd_mutex, UNLOCK);
	return (NULL);
}
