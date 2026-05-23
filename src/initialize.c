/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:55:33 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 12:52:28 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	assign_dongles(t_coder *coder, t_dongle *dongles, int coder_pos)
{
	int	total_coders;

	if (!coder || !dongles)
		return (1);
	total_coders = coder->cd->n_coders;
	if (coder_pos % 2 == 0)
	{
		coder->left_dongle = &dongles[coder_pos];
		coder->right_dongle = &dongles[(coder_pos + 1) % total_coders];
	}
	else
	{
		coder->left_dongle = &dongles[(coder_pos + 1) % total_coders];
		coder->right_dongle = &dongles[coder_pos];
	}
	return (0);
}

static int	coder_init_helper(t_coder *coder)
{
	coder->compile_count = 0;
	coder->debug_count = 0;
	coder->refactor_count = 0;
	coder->coder_work_done = false;
	coder->last_compile_t = 0;
	coder->coder_req.coder_id = coder->coder_id;
	coder->coder_req.arrival_t = 0;
	coder->coder_req.deadline_t = 0;
	if (mutex_safe(&coder->coder_mutex, INIT) != 0)
		return (1);
	coder->coder_mutex_initialized = true;
	return (0);
}

static int	coder_init(t_coding_data *cd)
{
	int		i;
	t_coder	*coder;

	i = -1;
	while (++i < cd->n_coders)
	{
		coder = &cd->coders[i];
		coder->cd = cd;
		coder->coder_id = i + 1;
		if (coder_init_helper(coder))
			return (1);
		if (assign_dongles(coder, cd->dongles, i))
			return (1);
	}
	return (0);
}

static int	init_all_dongles(t_coding_data *cd)
{
	int	i;

	i = -1;
	while (++i < cd->n_coders)
	{
		if (mutex_safe(&cd->dongles[i].dongle_mutex, INIT) != 0)
			return (1);
		cd->dongles[i].dongle_mutex_initialized = true;
		if (cond_safe(&cd->dongles[i].dongle_cond, NULL, INIT, NULL) != 0)
			return (1);
		if (init_dongle(&cd->dongles[i], cd))
			return (1);
		cd->dongles[i].dongle_id = i;
	}
	return (0);
}

int	data_init(t_coding_data *cd)
{
	if (NULL == cd || cd->n_coders <= 0 || cd->n_compiles <= 0)
		return (1);
	cd->coders = create_safe_malloc((cd->n_coders) * sizeof(t_coder));
	if (!cd->coders)
		return (1);
	cd->dongles = create_safe_malloc((cd->n_coders) * sizeof(t_dongle));
	if (!cd->dongles)
		return (1);
	if (mutex_safe(&cd->cd_mutex, INIT) != 0)
		return (1);
	cd->cd_mutex_initialized = true;
	if (mutex_safe(&cd->op_mutex, INIT) != 0)
		return (1);
	cd->op_mutex_initialized = true;
	if (!init_all_dongles(cd))
		if (!coder_init(cd))
			return (0);
	return (1);
}
