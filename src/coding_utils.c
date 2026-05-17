/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:58:22 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/17 15:55:29 by jmanani          ###   ########.fr       */
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
	while (!coding_finished(coder->cd))
	{
		if (get_bool(&coder->coder_mutex, &coder->coder_work_done))
			break ;
		compile(coder);
		if (!coding_finished(coder->cd))
		{
			coder->debug_count++;
			print_data(DEBUGGING, coder, DEBUG_MODE);
			updated_usleep(coder->cd, coder->cd->debug_time);
			coder->refactor_count++;
			print_data(REFACTORING, coder, DEBUG_MODE);
			updated_usleep(coder->cd, coder->cd->refactor_time);
		}
	}
	return (NULL);
}

// void	refactor(t_coder *coder)
// {
// 	if (!coder || !coder->cd || coding_finished(coder->cd))
// 		return ;
// 	coder->refactor_count++;
// 	print_data(REFACTORING, coder, DEBUG_MODE);
// 	updated_usleep(coder->cd, coder->cd->refactor_time);
// }

static void	compile_helper(t_coder *coder)
{
	acquire_dongle(coder->cd, coder->left_dongle);
	if (coding_finished(coder->cd))
	{
		release_dongle(coder->cd, coder->left_dongle);
		return ;
	}
	print_data(TOOK_DONGLE_1, coder, DEBUG_MODE);
	acquire_dongle(coder->cd, coder->right_dongle);
	if (coding_finished(coder->cd))
	{
		release_dongle(coder->cd, coder->left_dongle);
		release_dongle(coder->cd, coder->right_dongle);
		return ;
	}
	print_data(TOOK_DONGLE_2, coder, DEBUG_MODE);
	set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
	increase_long(&coder->coder_mutex, &coder->compile_count);
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

	if (!coder || !coder->cd)
		err_and_exit("Error: Null pointer in compile fn\n");
	if (coding_finished(coder->cd))
		return ;
	printf("Coder %d is requesting to compile.\n", coder->coder_id);
	req.coder_id = coder->coder_id;
	req.arrival_t = get_time(MILLISEC);
	req.deadline_t = req.arrival_t + (coder->cd->burn_time);
	printf("Coder  %d asking for CD Mutex\n", coder->coder_id);
	if (mutex_safe(&coder->cd->cd_mutex, LOCK) != 0)
		err_and_exit("Error: mutex_safe failed in compile fn\n");
	heap_push(coder->cd->algo_heap, req);
	printf("Coder %d pushed request to heap\n", coder->coder_id);
	printf("Coder %d asking for Arbiter Cond\n", coder->coder_id);
	if (cond_safe(&coder->cd->arbiter_cond, NULL, BROADCAST, NULL) != 0)
		err_and_exit("Error: cond_safe failed in compile fn\n");
	if (mutex_safe(&coder->cd->cd_mutex, UNLOCK) != 0)
		err_and_exit("Error: mutex_safe failed in compile fn\n");
	printf("Coder %d released CD Mutex\n", coder->coder_id);
	if (mutex_safe(&coder->coder_mutex, LOCK) != 0)
		err_and_exit("Error: mutex_safe failed in compile fn\n");
	printf("Coder %d has locked Coder Mutex.\n", coder->coder_id);
	printf("[TS %ld] Coder %d waiting on coder_req_cond (req_pending=%d)\n",
		get_time(MILLISEC), coder->coder_id, coder->req_pending);
	while (!coder->req_pending && !coding_finished(coder->cd))
	{
		if (cond_safe(&coder->coder_req_cond, &coder->coder_mutex, WAIT,
				NULL) != 0)
			err_and_exit("Error: cond_safe failed in compile fn\n");
	}
	printf("[TS %ld] Coder %d awoken for request (req_pending=%d)\n",
		get_time(MILLISEC), coder->coder_id, coder->req_pending);
	if (coding_finished(coder->cd))
	{
		coder->req_pending = false;
		mutex_safe(&coder->coder_mutex, UNLOCK);
		return ;
	}
	coder->req_pending = false;
	mutex_safe(&coder->coder_mutex, UNLOCK);
	compile_helper(coder);
}
