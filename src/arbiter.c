/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arbiter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:54:34 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 21:31:25 by jmanani          ###   ########.fr       */
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
			cond_safe(&cd->arbiter_cond, &cd->cd_mutex, WAIT, NULL);
		if (coding_finished(cd))
			break ;
		req = heap_pop(cd->algo_heap);
		mutex_safe(&cd->cd_mutex, UNLOCK);
		coder = &cd->coders[req.coder_id - 1];
		mutex_safe(&coder->coder_mutex, LOCK);
		coder->req_pending = true;
		cond_safe(&coder->coder_req_cond, NULL, BROADCAST, NULL);
		mutex_safe(&coder->coder_mutex, UNLOCK);
		mutex_safe(&cd->cd_mutex, LOCK);
	}
	mutex_safe(&cd->cd_mutex, UNLOCK);
	return (NULL);
}
