/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:55:33 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/14 13:29:32 by jmanani          ###   ########.fr       */
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

static void	coder_init(t_coding_data *cd)
{
	int		i;
	t_coder	*coder;

	i = -1;
	while (++i < cd->n_coders)
	{
		coder = &cd->coders[i];
		coder->coder_id = i + 1;
		coder->compile_count = 0;
		coder->debug_count = 0;
		coder->refactor_count = 0;
		coder->coder_work_done = false;
		coder->last_compile_t = 0;
		mutex_safe(&coder->coder_mutex, INIT);
		assign_dongles(coder, cd->dongles, i);
		coder->cd = cd;
	}
}

void	data_init(t_coding_data *cd)
{
	int	i;

	i = -1;
	printf("Initializing data...\n");
	if (NULL == cd)
		err_and_exit("Error: coding_data is NULL in data_init\n");
	cd->coders = malloc_safe_create(sizeof(t_coder) * cd->n_coders);
	cd->dongles = malloc_safe_create(sizeof(t_dongle) * cd->n_coders);
	mutex_safe(&cd->cd_mutex, INIT);
	while (++i < cd->n_coders)
	{
		mutex_safe(&cd->dongles[i].dongle_mutex, INIT);
		cd->dongles[i].dongle_id = i;
	}
	coder_init(cd);
	printf("Data initialization complete.\n");
}
