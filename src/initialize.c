/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:55:33 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 19:24:52 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	assign_dongles(t_coder *coder, t_dongle *dongles, int coder_pos)
{
	int	total_coders;

	if (!coder || !dongles)
		err_and_exit("Error: coder or dongles is NULL in assign_dongles\n");
	total_coders = coder->cd->n_coders;
	coder->left_dongle = &dongles[(coder_pos + 1) % total_coders];
	coder->right_dongle = &dongles[coder_pos];
	if (coder->coder_id % 2)
	{
		coder->left_dongle = &dongles[coder_pos];
		coder->right_dongle = &dongles[(coder_pos + 1) % total_coders];
	}
}

static void	coder_init(t_coding_data *cd)
{
	int		i;
	t_coder	*coder;

	i = -1;
	while (++i < cd->n_coders)
	{
		coder = &cd->coders[i];
		coder->cd = cd;
		coder->coder_id = i + 1;
		coder->compile_count = 0;
		coder->debug_count = 0;
		coder->refactor_count = 0;
		coder->coder_work_done = false;
		coder->last_compile_t = 0;
		mutex_safe(&coder->coder_mutex, INIT);
		if (pthread_cond_init(&coder->coder_req_cond, NULL) != 0)
			err_and_exit("Error: coder_init: pthread_cond_init failed\n");
		coder->arrival_t = 0;
		coder->deadline_t = 0;
		coder->req_pending = false;
		assign_dongles(coder, cd->dongles, i);
	}
}

void	data_init(t_coding_data *cd)
{
	int	i;

	i = -1;
	if (NULL == cd || cd->n_coders <= 0)
		return ;
	cd->coders = malloc_safe_create(sizeof(t_coder) * cd->n_coders);
	cd->dongles = malloc_safe_create(sizeof(t_dongle) * cd->n_coders);
	heap_init(&cd->algo_heap, cd->n_coders, cd->scheduler);
	mutex_safe(&cd->cd_mutex, INIT);
	mutex_safe(&cd->op_mutex, INIT);
	cond_safe(&cd->arbiter_cond, NULL, INIT, NULL);
	while (++i < cd->n_coders)
	{
		mutex_safe(&cd->dongles[i].dongle_mutex, INIT);
		mutex_safe(&cd->dongles[i].dongle_state_mutex, INIT);
		cond_safe(&cd->dongles[i].dongle_cond, NULL, INIT, NULL);
		cd->dongles[i].dongle_id = i;
		cd->dongles[i].next_available_t = 0;
	}
	coder_init(cd);
}
