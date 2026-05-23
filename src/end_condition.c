/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_condition.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 10:25:33 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 13:53:28 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wake_dongles(t_coding_data *cd)
{
	int	i;

	i = -1;
	if (!cd || !cd->n_coders || !cd->dongles)
		return ;
	while (++i < cd->n_coders)
	{
		mutex_safe(&cd->dongles[i].dongle_mutex, LOCK);
		cond_safe(&cd->dongles[i].dongle_cond, NULL, BROADCAST, NULL);
		mutex_safe(&cd->dongles[i].dongle_mutex, UNLOCK);
	}
}

static void	wake_global(t_coding_data *cd)
{
	if (!cd || !cd->global_cond_initialized)
		return ;
	mutex_safe(&cd->global_mutex, LOCK);
	cond_safe(&cd->global_cond, NULL, BROADCAST, NULL);
	mutex_safe(&cd->global_mutex, UNLOCK);
}

void	signal_end(t_coding_data *cd)
{
	if (!cd)
		return ;
	set_bool(&cd->cd_mutex, &cd->end_coding, true);
	wake_dongles(cd);
	wake_global(cd);
}

void	mark_coder_done(t_coder *coder)
{
	long	count;

	if (!coder || !coder->cd)
		return ;
	set_bool(&coder->coder_mutex, &coder->coder_work_done, true);
	mutex_safe(&coder->cd->cd_mutex, LOCK);
	coder->cd->done_coders++;
	count = coder->cd->done_coders;
	mutex_safe(&coder->cd->cd_mutex, UNLOCK);
	if (count == coder->cd->n_coders)
		signal_end(coder->cd);
}
