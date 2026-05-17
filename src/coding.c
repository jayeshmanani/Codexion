/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:19:42 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/17 15:56:39 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coding_sim(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	if (NULL == coder)
		return (NULL);
	printf("Coder %d is ready to start coding.\n", coder->coder_id);
	printf("Coder %d Will Need %d Dongle, %d Dongle\n", coder->coder_id,
		coder->left_dongle->dongle_id, coder->right_dongle->dongle_id);
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
			increase_long(&coder->coder_mutex, &coder->refactor_count);
			print_data(REFACTORING, coder, DEBUG_MODE);
			updated_usleep(coder->cd, coder->cd->refactor_time);
		}
	}
	printf("Coder %d has finished coding.\n", coder->coder_id);
	return (NULL);
}

void	coding_helper(t_coding_data *cd)
{
	int	i;

	i = -1;
	printf("Coding helper initialized.\n");
	if (NULL == cd || cd->n_coders <= 0)
		return ;
	set_long(&cd->cd_mutex, &cd->start_coding_t, get_time(MILLISEC));
	set_bool(&cd->cd_mutex, &cd->coders_ready, true);
	thread_safe(&cd->analyzer, CREATE, coding_analyser, cd);
	i = -1;
	while (++i < cd->n_coders)
	{
		printf("[HELPER] about to JOIN coder index=%d id=%d tid=%lu\n", i,
			cd->coders[i].coder_id, (unsigned long)cd->coders[i].c_thread_id);
		thread_safe(&cd->coders[i].c_thread_id, JOIN, NULL, NULL);
		printf("[HELPER] done JOIN coder index=%d id=%d tid=%lu\n", i,
			cd->coders[i].coder_id, (unsigned long)cd->coders[i].c_thread_id);
	}
	set_bool(&cd->cd_mutex, &cd->end_coding, true);
	if (cond_safe(&cd->arbiter_cond, NULL, BROADCAST, NULL) != 0)
		err_and_exit("Error: cond_safe failed in coding_helper\n");
	if (cd->n_coders > 1)
	{
		printf("[HELPER] about to JOIN arbiter tid=%lu\n",
			(unsigned long)cd->arbiter);
		thread_safe(&cd->arbiter, JOIN, NULL, NULL);
		printf("[HELPER] done JOIN arbiter tid=%lu\n",
			(unsigned long)cd->arbiter);
	}
	printf("[HELPER] about to JOIN analyzer tid=%lu\n",
		(unsigned long)cd->analyzer);
	thread_safe(&cd->analyzer, JOIN, NULL, NULL);
	printf("[HELPER] done JOIN analyzer tid=%lu\n",
		(unsigned long)cd->analyzer);
	printf("Coding helper ended.\n");
}

int	coding_start(t_coding_data *cd)
{
	int	i;

	i = -1;
	printf("Starting the coding simulation...\n");
	if (NULL == cd || cd->n_coders <= 0)
		return (1);
	if (1 == cd->n_coders)
		thread_safe(&cd->coders[0].c_thread_id, CREATE, lone_vibe_coder,
			&cd->coders[0]);
	else
	{
		thread_safe(&cd->arbiter, CREATE, arbiter_thread, cd);
		printf("Coders: %ld\n", cd->n_coders);
		while (++i < cd->n_coders)
			thread_safe(&cd->coders[i].c_thread_id, CREATE, coding_sim,
				&cd->coders[i]);
	}
	coding_helper(cd);
	printf("Coding simulation ended.\n");
	return (0);
}
