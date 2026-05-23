/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 16:01:46 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 20:42:17 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_destroy(t_heap *heap)
{
	if (!heap)
		return ;
	if (heap->arr)
		free(heap->arr);
	heap->arr = NULL;
	free(heap);
}

int	heap_remove(t_heap *heap, int coder_id)
{
	long	i;

	if (!heap || heap->size <= 0 || coder_id <= 0)
		return (1);
	i = -1;
	while (++i < heap->size)
	{
		if (heap->arr[i].coder_id == coder_id)
		{
			heap->size--;
			if (i < heap->size)
			{
				heap->arr[i] = heap->arr[heap->size];
				up_shift(heap, i);
				down_shift(heap, i);
			}
			return (0);
		}
	}
	return (0);
}
