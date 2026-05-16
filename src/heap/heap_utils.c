/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 16:01:46 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 21:40:32 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	heap_is_empty(t_coding_data *cd)
{
	if (!cd || !cd->algo_heap)
		return (true);
	return (cd->algo_heap->size == 0);
}

void	heap_destroy(t_coding_data *cd)
{
	if (!cd || !cd->algo_heap)
		return ;
	free(cd->algo_heap->arr);
	cd->algo_heap->arr = NULL;
	free(cd->algo_heap);
	cd->algo_heap = NULL;
}
