/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:06:25 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 22:55:07 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int destroy_coder(t_coder *coder)
{
	if (!coder)
		return (1);
	if (coder->coder_mutex_initialized)
		mutex_safe(&coder->coder_mutex, DESTROY);
	if (coder->coder_req_cond_initialized)
		if (cond_safe(&coder->coder_req_cond, NULL, DESTROY, NULL) != 0)
			return (1);
	return (0);
}

int destroy_dongle(t_dongle *dongle)
{
	if (!dongle)
		return (1);
	if (dongle->dongle_mutex_initialized)
		mutex_safe(&dongle->dongle_mutex, DESTROY);
	if (dongle->dongle_state_mutex_initialized)
		mutex_safe(&dongle->dongle_state_mutex, DESTROY);
	if (dongle->dongle_cond_initialized)
		if (cond_safe(&dongle->dongle_cond, NULL, DESTROY, NULL) != 0)
			return (1);
	return (0);
}

void	clean_all(t_coding_data *cd)
{
	int	i;

	i = -1;
	if (NULL == cd || cd->n_coders <= 0)
		return ;
	while (++i < cd->n_coders)
	{
		destroy_coder(&cd->coders[i]);
		destroy_dongle(&cd->dongles[i]);
	}
	free(cd->coders);
	free(cd->dongles);
	if (cd->arbiter_cond_initialized)
		if (cond_safe(&cd->arbiter_cond, NULL, DESTROY, NULL) != 0)
			err_and_exit("Error: cond_safe failed in clean_all\n");
	if (cd->op_mutex_initialized)
		mutex_safe(&cd->op_mutex, DESTROY);	
	if (cd->cd_mutex_initialized)
		mutex_safe(&cd->cd_mutex, DESTROY);
	heap_destroy(cd);
}

void	err_and_exit(const char *error)
{
	if (NULL == error)
		error = "ErrorEnd: An unknown error occurred.\n";
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}
