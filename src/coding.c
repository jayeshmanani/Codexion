/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:19:42 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/20 21:01:32 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	coding_sim_helper(t_coder *coder)
{
	increase_long(&coder->coder_mutex, &coder->debug_count);
	print_data(DEBUGGING, coder);
	updated_usleep(coder->cd, coder->cd->debug_time);
	increase_long(&coder->coder_mutex, &coder->refactor_count);
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
	coder->coder_req.arrival_t = get_time(MILLISEC);
	waiting_for_coders(coder->cd);
	set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
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

// void	*coding_sim(void *args)
// {
// 	t_coder	*coder;

// 	coder = (t_coder *)args;
// 	if (NULL == coder)
// 		return (NULL);
// waiting_for_coders(coder->cd);
// set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
// increase_long(&coder->cd->cd_mutex, &coder->cd->active_coders);
// 	while (!coding_finished(coder->cd))
// 	{
// 		if (get_bool(&coder->coder_mutex, &coder->coder_work_done))
// 			break ;
// 		if (compile(coder) != 0)
// 			return (NULL);
// 		if (get_bool(&coder->coder_mutex, &coder->coder_work_done))
// 			break ;
// 		if (!coding_finished(coder->cd))
// 		{
// 			coding_sim_helper(coder);
// 		}
// 	}
// 	return (NULL);
// }

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
	if (NULL == cd || cd->n_coders <= 0)
		return (1);
	while (++i < cd->n_coders)
	{
		set_long(&cd->cd_mutex, &cd->start_coding_t, get_time(MILLISEC));
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
