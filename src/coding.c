/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:19:42 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 19:57:26 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	coding_sim_helper(t_coder *coder)
{
	print_data(DEBUGGING, coder);
	updated_usleep(coder->cd, coder->cd->debug_time);
	print_data(REFACTORING, coder);
	updated_usleep(coder->cd, coder->cd->refactor_time);
	return (0);
}

void	*coding_sim(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	if (NULL == coder)
		return (NULL);
	waiting_for_coders(coder->cd);
	increase_long(&coder->cd->cd_mutex, &coder->cd->active_coders);
	if (coder->coder_id % 2 == 0)
		usleep(1000);
	while (!coding_finished(coder->cd))
	{
		if (get_bool(&coder->coder_mutex, &coder->coder_work_done))
			break ;
		if (compile(coder) != 0)
			return (NULL);
		if (get_bool(&coder->coder_mutex, &coder->coder_work_done))
			break ;
		if (!coding_finished(coder->cd))
		{
			coding_sim_helper(coder);
		}
	}
	return (NULL);
}

void	coding_helper(t_coding_data *cd)
{
	int	i;
	int	analyser_created;

	analyser_created = 1;
	if (NULL == cd || cd->n_coders <= 0)
		return ;
	set_bool(&cd->cd_mutex, &cd->coders_ready, true);
	if (thread_safe(&cd->analyzer, CREATE, coding_analyser, cd) != 0)
	{
		set_bool(&cd->cd_mutex, &cd->end_coding, true);
		analyser_created = 0;
	}
	i = -1;
	while (++i < cd->n_coders)
		thread_safe(&cd->coders[i].c_thread_id, JOIN, NULL, NULL);
	set_bool(&cd->cd_mutex, &cd->end_coding, true);
	while (--i >= 0)
	{
		mutex_safe(&cd->dongles[i].dongle_mutex, LOCK);
		cond_safe(&cd->dongles[i].dongle_cond, NULL, BROADCAST, NULL);
		mutex_safe(&cd->dongles[i].dongle_mutex, UNLOCK);
	}
	if (analyser_created)
		thread_safe(&cd->analyzer, JOIN, NULL, NULL);
}

int	coding_start(t_coding_data *cd)
{
	int	i;

	i = -1;
	if (NULL == cd || cd->n_coders <= 0 || cd->n_compiles <= 0)
		return (1);
	while (++i < cd->n_coders)
	{
		if (thread_safe(&cd->coders[i].c_thread_id, CREATE, coding_sim,
				&cd->coders[i]) != 0)
		{
			set_bool(&cd->cd_mutex, &cd->end_coding, true);
			while (--i >= 0)
				thread_safe(&cd->coders[i].c_thread_id, JOIN, NULL, NULL);
			return (1);
		}
	}
	coding_helper(cd);
	return (0);
}
