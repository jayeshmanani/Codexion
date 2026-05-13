/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:19:42 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/13 19:00:53 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	data_print(t_coding_data *cd)
{
	int	i;

	i = -1;
	while (++i < cd->n_coders)
	{
		printf("ID: %d\n", cd->coders[i].coder_id);
		printf("Thread ID: %ld\n", cd->coders[i].coder_thread_id);
	}
}

void	*coding_sim(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	printf("Coder: %d\n", coder->coder_id);
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
	data_print(cd);
	while (++i < cd->n_coders)
		thread_safe(&cd->coders[i].coder_thread_id, CREATE, coding_sim,
			&cd->coders[i]);
	data_print(cd);
}
