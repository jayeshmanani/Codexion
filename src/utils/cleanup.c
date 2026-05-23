/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:06:25 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 13:25:30 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	clean_globals(t_coding_data *cd)
{
	if (cd->global_cond_initialized)
	{
		cond_safe(&cd->global_cond, NULL, DESTROY, NULL);
		cd->global_cond_initialized = false;
	}
	if (cd->global_mutex_initialized)
	{
		mutex_safe(&cd->global_mutex, DESTROY);
		cd->global_mutex_initialized = false;
	}
	if (cd->global_heap)
	{
		heap_destroy(cd->global_heap);
		cd->global_heap = NULL;
	}
}

static void	clean_mutexes(t_coding_data *cd)
{
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

void	clean_all(t_coding_data *cd)
{
	if (NULL == cd)
		return ;
	clean_globals(cd);
	destroy_all_coders(cd);
	destroy_all_dongles(cd);
	clean_mutexes(cd);
}

void	err_and_exit(const char *error)
{
	if (NULL == error)
		error = "ErrorEnd: An unknown error occurred.\n";
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}

void	print_error(const char *error)
{
	if (NULL == error)
		error = "Error: An unknown error occurred.\n";
	printf("%s\n", error);
}
