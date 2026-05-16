/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:06:25 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 20:13:29 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	clean_all(t_coding_data *cd)
{
	int	i;

	i = -1;
	if (NULL == cd || cd->n_coders <= 0)
		return ;
	while (++i < cd->n_coders)
	{
		mutex_safe(&cd->coders[i].coder_mutex, DESTROY);
		cond_safe(&cd->coders[i].coder_req_cond, NULL, DESTROY, NULL);
		mutex_safe(&cd->dongles[i].dongle_mutex, DESTROY);
		mutex_safe(&cd->dongles[i].dongle_state_mutex, DESTROY);
		cond_safe(&cd->dongles[i].dongle_cond, NULL, DESTROY, NULL);
	}
	cond_safe(&cd->arbiter_cond, NULL, DESTROY, NULL);
	mutex_safe(&cd->cd_mutex, DESTROY);
	mutex_safe(&cd->op_mutex, DESTROY);
	heap_destroy(&cd->algo_heap);
	free(cd->coders);
	free(cd->dongles);
}

void	err_and_exit(const char *error)
{
	if (NULL == error)
		error = "ErrorEnd: An unknown error occurred.\n";
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}
