/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:06:25 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/17 14:50:24 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	clean_all(t_coding_data *cd)
{
	if (NULL == cd)
		return ;
	destroy_all_coders(cd);
	destroy_all_dongles(cd);
	heap_destroy(cd);
	if (cd->arbiter_cond_initialized)
	{
		if (cond_safe(&cd->arbiter_cond, NULL, DESTROY, NULL) != 0)
			err_and_exit("Error: cond_safe failed in clean_all\n");
	}
	if (cd->op_mutex_initialized)
	{
		mutex_safe(&cd->op_mutex, DESTROY);
		cd->op_mutex_initialized = false;
	}
	if (cd->cd_mutex_initialized)
	{
		mutex_safe(&cd->cd_mutex, DESTROY);
		cd->cd_mutex_initialized = false;
	}
}

void	err_and_exit(const char *error)
{
	if (NULL == error)
		error = "ErrorEnd: An unknown error occurred.\n";
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}
