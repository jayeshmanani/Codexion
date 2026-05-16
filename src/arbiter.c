/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arbiter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:54:34 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 23:25:51 by jmanani          ###   ########.fr       */
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
	printf("Arbiter thread initialized. Locked CD Mutex\n");
	while (!coding_finished(cd))
	{
		printf("Arbiter Trying While\n");
		while (!coding_finished(cd) && heap_is_empty(cd))
			if (cond_safe(&cd->arbiter_cond, &cd->cd_mutex, WAIT, NULL) != 0)
				err_and_exit("Error: cond_safe failed in arbiter_thread\n");
		printf("Arbiter Waiting for Request\n");
		if (!coding_finished(cd))
		{
			req = heap_pop(cd->algo_heap);
			if (mutex_safe(&cd->cd_mutex, UNLOCK) != 0)
				err_and_exit("Error: mutex_safe failed in arbiter_thread\n");
			coder = &cd->coders[req.coder_id - 1];
			if (mutex_safe(&coder->coder_mutex, LOCK) != 0)
				err_and_exit("Error: mutex_safe failed in arbiter_thread\n");
			coder->req_pending = true;
			printf("Arbiter Broadcasting Request\n");
			if (cond_safe(&coder->coder_req_cond, NULL, BROADCAST, NULL) != 0)
				err_and_exit("Error: cond_safe failed in arbiter_thread\n");
			
			if (mutex_safe(&coder->coder_mutex, UNLOCK) != 0)
				err_and_exit("Error: mutex_safe failed in arbiter_thread\n");
			printf("Arbiter Released Coder Mutex\n");
			if (mutex_safe(&cd->cd_mutex, LOCK) != 0)
				err_and_exit("Error: mutex_safe failed in arbiter_thread\n");
			printf("Arbiter Locked CD Mutex\n");
		}
	}	
	mutex_safe(&cd->cd_mutex, UNLOCK);
	printf("Arbiter thread exiting. UNlocked CD Mutex\n");
	return (NULL);
}
