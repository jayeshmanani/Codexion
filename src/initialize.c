/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:55:33 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/15 16:52:24 by jmanani          ###   ########.fr       */
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
		assign_dongles(coder, cd->dongles, i);
	}
}

void	data_init(t_coding_data *cd)
{
	int	i;

	i = -1;
	if (NULL == cd)
		err_and_exit("Error: coding_data is NULL in data_init\n");
	cd->coders = malloc_safe_create(sizeof(t_coder) * cd->n_coders);
	cd->dongles = malloc_safe_create(sizeof(t_dongle) * cd->n_coders);
	mutex_safe(&cd->cd_mutex, INIT);
	mutex_safe(&cd->op_mutex, INIT);
	while (++i < cd->n_coders)
	{
		mutex_safe(&cd->dongles[i].dongle_mutex, INIT);
		mutex_safe(&cd->dongles[i].dongle_state_mutex, INIT);
		if (pthread_cond_init(&cd->dongles[i].dongle_cond, NULL) != 0)
			err_and_exit("Error: data_init: pthread_cond_init failed\n");
		cd->dongles[i].dongle_id = i;
		cd->dongles[i].next_available_t = 0;
	}
	coder_init(cd);
}
