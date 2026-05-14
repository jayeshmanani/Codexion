/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:55:33 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/14 12:21:25 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	assign_dongles(t_coder *coder, t_dongle *dongles, int coder_pos)
{
	int	total_coders;

	if (NULL == coder || NULL == dongles)
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

void	data_init(t_coding_data *cd)
{
	int	i;

	i = -1;
	if (NULL == cd)
		err_and_exit("Error: coding_data is NULL in data_init\n");
	cd->coders = malloc_safe_create(sizeof(t_coder) * cd->n_coders);
	cd->dongles = malloc_safe_create(sizeof(t_dongle) * cd->n_coders);
	mutex_safe(&cd->cd_mutex, INIT);
	while (++i < cd->n_coders)
	{
		mutex_safe(&cd->dongles[i].dongle, INIT);
		cd->dongles[i].dongle_id = i;
	}
	while (--i > -1)
	{
		cd->coders[i].coder_id = i + 1;
		cd->coders[i].compile_count = 0;
		cd->coders[i].debug_count = 0;
		cd->coders[i].refactor_count = 0;
		cd->coders[i].coder_work_done = false;
		cd->coders[i].last_compile_start = 0;
		cd->coders[i].cd = cd;
		assign_dongles(&cd->coders[i], cd->dongles, i);
	}
}
