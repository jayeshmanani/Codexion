/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:19:42 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/14 15:50:14 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*lone_vibe_coder(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	waiting_for_coders(coder->cd);
	set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
	print_data(TOOK_DONGLE_1, coder, DEBUG_MODE);
	while (!coding_finished(coder->cd))
		usleep(200);
	return (NULL);
}

static void	refactor(t_coder *coder)
{
	coder->refactor_count++;
	print_data(REFACTORING, coder, DEBUG_MODE);
	updated_usleep(coder->cd, coder->cd->refactor_time);
}

static void	compile(t_coder *coder)
{
	mutex_safe(&coder->left_dongle->dongle_mutex, LOCK);
	print_data(TOOK_DONGLE_1, coder, DEBUG_MODE);
	mutex_safe(&coder->right_dongle->dongle_mutex, LOCK);
	print_data(TOOK_DONGLE_2, coder, DEBUG_MODE);
	set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
	coder->compile_count++;
	print_data(COMPILING, coder, DEBUG_MODE);
	updated_usleep(coder->cd, coder->cd->compile_time);
	if (coder->cd->n_compiles > 0
		&& coder->compile_count == coder->cd->n_compiles)
		set_bool(&coder->coder_mutex, &coder->coder_work_done, true);
	mutex_safe(&coder->left_dongle->dongle_mutex, UNLOCK);
	mutex_safe(&coder->right_dongle->dongle_mutex, UNLOCK);
}

void	*coding_sim(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	waiting_for_coders(coder->cd);
	set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
	while (!coding_finished(coder->cd))
	{
		if (get_bool(&coder->coder_mutex, &coder->coder_work_done))
			break ;
		compile(coder);
		coder->debug_count++;
		print_data(DEBUGGING, coder, DEBUG_MODE);
		updated_usleep(coder->cd, coder->cd->debug_time);
		refactor(coder);
	}
	return (NULL);
}

void	coding_start(t_coding_data *cd)
{
	int	i;

	i = -1;
	if (NULL == cd)
		err_and_exit("Error: coding_data is NULL or 0 Compile needed\n");
	else if (0 == cd->n_compiles)
		return ;
	else if (1 == cd->n_coders)
		thread_safe(&cd->coders[0].coder_thread_id, CREATE, lone_vibe_coder,
			&cd->coders[0]);
	else
	{
		printf("Coders: %ld\n", cd->n_coders);
		while (++i < cd->n_coders)
			thread_safe(&cd->coders[i].coder_thread_id, CREATE, coding_sim,
				&cd->coders[i]);
	}
	thread_safe(&cd->analyzer, CREATE, coding_analyser, cd);
	cd->start_coding_time = get_time(MILLISEC);
	set_bool(&cd->cd_mutex, &cd->coders_ready, true);
	i = -1;
	while (++i < cd->n_coders)
		thread_safe(&cd->coders[i].coder_thread_id, JOIN, NULL, NULL);
}
