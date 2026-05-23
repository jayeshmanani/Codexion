/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:55:00 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 13:55:00 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_global_queue(t_coding_data *cd)
{
	if (!cd)
		return (1);
	cd->global_heap = malloc(sizeof(t_heap));
	if (!cd->global_heap)
		return (1);
	cd->global_heap->arr = malloc(cd->n_coders * sizeof(t_req));
	if (!cd->global_heap->arr)
	{
		free(cd->global_heap);
		cd->global_heap = NULL;
		return (1);
	}
	cd->global_heap->size = 0;
	cd->global_heap->capacity = cd->n_coders;
	cd->global_heap->scheduler = cd->scheduler;
	if (mutex_safe(&cd->global_mutex, INIT) != 0)
		return (1);
	cd->global_mutex_initialized = true;
	if (cond_safe(&cd->global_cond, NULL, INIT, NULL) != 0)
		return (1);
	cd->global_cond_initialized = true;
	return (0);
}
