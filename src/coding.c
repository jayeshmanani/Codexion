/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:19:42 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/14 12:09:40 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coding_sim(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	waiting_for_coders(coder->cd);
	printf("Coder %d is starting to code\n", coder->coder_id);
	while (!coding_finished(coder->cd))
	{
		if (coder->coder_work_done) // to do mutex
			break ;
		// compile
		// debug
		// refractor
	}
	return (NULL);
}

void	coding_start(t_coding_data *cd)
{
	int	i;

	i = -1;
	if ((NULL == cd) || (0 == cd->n_compiles))
		err_and_exit("Error: coding_data is NULL or 0 Compile needed\n");
	else if (1 == cd->n_coders)
		err_and_exit("Error: one coders only\n");
	else
	{
		printf("Coders: %ld\n", cd->n_coders);
		while (++i < cd->n_coders)
			thread_safe(&cd->coders[i].coder_thread_id, CREATE, coding_sim,
				&cd->coders[i]);
	}
	cd->start_coding_time = get_time(MILLISEC);
	set_bool(&cd->cd_mutex, &cd->coders_ready, true);
	i = -1;
	while (++i < cd->n_coders)
		thread_safe(&cd->coders[i].coder_thread_id, JOIN, NULL, NULL);
}
