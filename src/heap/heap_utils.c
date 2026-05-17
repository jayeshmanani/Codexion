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

bool	heap_is_empty(t_heap *heap)
{
	if (!heap)
		return (true);
	return (heap->size == 0);
}

void	heap_destroy(t_heap *heap)
{
	if (!heap)
		return ;
	if (heap->arr)
		free(heap->arr);
	heap->arr = NULL;
	free(heap);
}
