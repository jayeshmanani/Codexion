/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:58:22 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 16:35:57 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*lone_vibe_coder(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	waiting_for_coders(coder->cd);
	increase_long(&coder->cd->cd_mutex, &coder->cd->active_coders);
	set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
	print_data(TOOK_DONGLE_1, coder, DEBUG_MODE);
	while (!coding_finished(coder->cd))
		usleep(200);
	return (NULL);
}

void	refactor(t_coder *coder)
{
	coder->refactor_count++;
	print_data(REFACTORING, coder, DEBUG_MODE);
	updated_usleep(coder->cd, coder->cd->refactor_time);
}

static void compile_helper(t_coder *coder){
	acquire_dongle(coder->cd, coder->left_dongle);
    print_data(TOOK_DONGLE_1, coder, DEBUG_MODE);
    acquire_dongle(coder->cd, coder->right_dongle);
    print_data(TOOK_DONGLE_2, coder, DEBUG_MODE);
    set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
    coder->compile_count++;
    print_data(COMPILING, coder, DEBUG_MODE);
    updated_usleep(coder->cd, coder->cd->compile_time);
    if (coder->cd->n_compiles > 0
        && coder->compile_count == coder->cd->n_compiles)
        set_bool(&coder->coder_mutex, &coder->coder_work_done, true);
    release_dongle(coder->cd, coder->left_dongle);
    release_dongle(coder->cd, coder->right_dongle);
}

void	compile(t_coder *coder)
{
	t_req	req;

    req.coder_id = coder->coder_id;
    req.arrival_t = get_time(MILLISEC);
    req.deadline_t = req.arrival_t + (coder->cd->burn_time);

	mutex_safe(&coder->cd->cd_mutex, LOCK);
    heap_push(&coder->cd->algo_heap, req);
    if (pthread_cond_broadcast(&coder->cd->arbiter_cond) != 0)
        err_and_exit("Error: cond_broadcast failed in compile\n");
    mutex_safe(&coder->cd->cd_mutex, UNLOCK);

	mutex_safe(&coder->coder_mutex, LOCK);
    while (!coder->req_pending && !coding_finished(coder->cd))
        pthread_cond_wait(&coder->coder_req_cond, &coder->coder_mutex);
    coder->req_pending = false;
    mutex_safe(&coder->coder_mutex, UNLOCK);
	compile_helper(coder);
}
