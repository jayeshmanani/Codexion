/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:55:33 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 22:47:21 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// static int	assign_dongles(t_coder *coder, t_dongle *dongles, int coder_pos)
// {
// 	int	total_coders;

// 	if (!coder || !dongles)
// 		return (1);
// 	total_coders = coder->cd->n_coders;
// 	coder->left_dongle = &dongles[(coder_pos + 1) % total_coders];
// 	coder->right_dongle = &dongles[coder_pos];
// 	if (coder->coder_id % 2)
// 	{
// 		coder->left_dongle = &dongles[coder_pos];
// 		coder->right_dongle = &dongles[(coder_pos + 1) % total_coders];
// 	}
// 	return (0);
// }

static int	coder_init_helper(t_coder *coder)
{
	coder->compile_count = 0;
	coder->debug_count = 0;
	coder->refactor_count = 0;
	coder->coder_work_done = false;
	coder->last_compile_t = 0;
	if (mutex_safe(&coder->coder_mutex, INIT) != 0)
		return (1);
	coder->coder_mutex_initialized = true;
	if (cond_safe(&coder->coder_req_cond, NULL, INIT, NULL) != 0)
		return (1);
	coder->coder_req_cond_initialized = true;
	coder->arrival_t = 0;
	coder->deadline_t = 0;
	coder->req_pending = false;
	return (0);
}

static int	coder_init(t_coding_data *cd)
{
	int		i;
	t_coder	*coder;

	i = -1;
	printf("Coders init started\n");
	while (++i < cd->n_coders)
	{
		coder = &cd->coders[i];
		coder->cd = cd;
		coder->coder_id = i + 1;
		if (coder_init_helper(coder))
			return (1);
		// if (assign_dongles(coder, cd->dongles, i))
		// 	return (1);
	}
	printf("Coders init done\n");
	return (0);
}

// int	init_malloc_mem(t_coding_data *cd)
// {
// 	if (malloc_safe_create(cd, 'c') || malloc_safe_create(cd, 'h')
// 		|| malloc_safe_create(cd, 'd'))
// 		return (1);
// 	cd->algo_heap->size = 0;
// 	cd->algo_heap->capacity = cd->n_coders;
// 	cd->algo_heap->scheduler = cd->scheduler;
// 	return (0);
// }

static int	dongle_init(t_coding_data *cd)
{
	int	i;

	i = -1;
	printf("Dongles init started\n");
	while (++i < cd->n_coders)
	{
		if (mutex_safe(&cd->dongles[i].dongle_mutex, INIT) != 0)
			return (1);
		cd->dongles[i].dongle_mutex_initialized = true;
		if (mutex_safe(&cd->dongles[i].dongle_state_mutex, INIT) != 0)
			return (1);
		cd->dongles[i].dongle_state_mutex_initialized = true;
		if (cond_safe(&cd->dongles[i].dongle_cond, NULL, INIT, NULL) != 0)
			return (1);
		cd->dongles[i].dongle_cond_initialized = true;
		cd->dongles[i].dongle_id = i;
		cd->dongles[i].next_available_t = 0;
	}
	printf("Dongles init done\n");
	return (0);
}

int	data_init(t_coding_data *cd)
{
	printf("Data init started\n");
	if (NULL == cd || cd->n_coders <= 0)
		return (1);
	printf("Mallocing mem\n");
	if (malloc_safe_create(cd, 'c') || malloc_safe_create(cd, 'h')
		|| malloc_safe_create(cd, 'd'))
		return (1);
	printf("Mallocing mem done\n");
	cd->algo_heap->size = 0;
	cd->algo_heap->capacity = cd->n_coders;
	cd->algo_heap->scheduler = cd->scheduler;
	if (mutex_safe(&cd->cd_mutex, INIT) != 0)
		return (1);
	cd->cd_mutex_initialized = true;
	if (mutex_safe(&cd->op_mutex, INIT) != 0)
		return (1);
	cd->op_mutex_initialized = true;
	if (cond_safe(&cd->arbiter_cond, NULL, INIT, NULL) != 0)
		return (1);
	cd->arbiter_cond_initialized = true;
	printf("Mutexes and conds init done - data init\n");
	if (!dongle_init(cd))
		if (!coder_init(cd))
			return (0);
	return (1);
}
