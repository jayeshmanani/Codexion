/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:06:25 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/15 16:58:12 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	clean_all(t_coding_data *cd)
{
	int	i;

	i = -1;
	if (NULL == cd)
		err_and_exit("Error - CleanALL: coding_data is NULL in clean_all\n");
	while (++i < cd->n_coders)
	{
		mutex_safe(&cd->coders[i].coder_mutex, DESTROY);
		mutex_safe(&cd->dongles[i].dongle_mutex, DESTROY);
		mutex_safe(&cd->dongles[i].dongle_state_mutex, DESTROY);
		if (pthread_cond_destroy(&cd->dongles[i].dongle_cond) != 0)
			err_and_exit("Error: clean_all: pthread_cond_destroy failed\n");
	}
	mutex_safe(&cd->cd_mutex, DESTROY);
	mutex_safe(&cd->op_mutex, DESTROY);
	free(cd->coders);
	free(cd->dongles);
	// free(cd->heap.arr);
}

void	err_and_exit(const char *error)
{
	if (NULL == error)
		error = "ErrorEnd: An unknown error occurred.\n";
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}
