/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:19:42 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/17 19:28:02 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coding_sim(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	if (NULL == coder)
		return (NULL);
	waiting_for_coders(coder->cd);
	set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
	increase_long(&coder->cd->cd_mutex, &coder->cd->active_coders);
	while (!coding_finished(coder->cd))
	{
		if (get_bool(&coder->coder_mutex, &coder->coder_work_done))
			break ;
		compile(coder);
		if (!coding_finished(coder->cd))
		{
			increase_long(&coder->coder_mutex, &coder->debug_count);
			updated_usleep(coder->cd, coder->cd->debug_time);
			print_data(DEBUGGING, coder, DEBUG_MODE);
			increase_long(&coder->coder_mutex, &coder->refactor_count);
			print_data(REFACTORING, coder, DEBUG_MODE);
			updated_usleep(coder->cd, coder->cd->refactor_time);
		}
	}
	return (NULL);
}

void	coding_helper(t_coding_data *cd)
{
	int	i;

	i = -1;
	if (NULL == cd || cd->n_coders <= 0)
		return ;
	set_long(&cd->cd_mutex, &cd->start_coding_t, get_time(MILLISEC));
	set_bool(&cd->cd_mutex, &cd->coders_ready, true);
	thread_safe(&cd->analyzer, CREATE, coding_analyser, cd);
	i = -1;
	while (++i < cd->n_coders)
		thread_safe(&cd->coders[i].c_thread_id, JOIN, NULL, NULL);
	set_bool(&cd->cd_mutex, &cd->end_coding, true);
	i = -1;
	while (++i < cd->n_coders)
		cond_safe(&cd->dongles[i].dongle_cond, NULL, BROADCAST, NULL);
	thread_safe(&cd->analyzer, JOIN, NULL, NULL);
}

int	coding_start(t_coding_data *cd)
{
	int	i;

	i = -1;
	if (NULL == cd || cd->n_coders <= 0)
		return (1);
	if (1 == cd->n_coders)
		thread_safe(&cd->coders[0].c_thread_id, CREATE, lone_vibe_coder,
			&cd->coders[0]);
	else
	{
		printf("Coders: %ld\n", cd->n_coders);
		while (++i < cd->n_coders)
			thread_safe(&cd->coders[i].c_thread_id, CREATE, coding_sim,
				&cd->coders[i]);
	}
	coding_helper(cd);
	return (0);
}
