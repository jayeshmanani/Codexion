/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 15:51:46 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 21:44:07 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	up_shift(t_heap *heap, long index)
{
	long	parent;

	while (index > 0)
	{
		parent = heap_parent(index);
		if (!find_preference(heap->arr[index], heap->arr[parent],
				heap->scheduler))
			break ;
		req_swap(&heap->arr[index], &heap->arr[parent]);
		index = parent;
	}
}

void	down_shift(t_heap *heap, long index)
{
	long	left;
	long	right;
	long	best;

	while (1)
	{
		left = heap_left(index);
		right = heap_right(index);
		best = index;
		if (left < heap->size && find_preference(heap->arr[left],
				heap->arr[best], heap->scheduler))
			best = left;
		if (right < heap->size && find_preference(heap->arr[right],
				heap->arr[best], heap->scheduler))
			best = right;
		if (best == index)
			break ;
		req_swap(&heap->arr[index], &heap->arr[best]);
		index = best;
	}
}

int	heap_push(t_heap *heap, t_req req)
{
	if (!heap || heap->size >= heap->capacity)
	{
		print_error("heap_push: full or invalid heap");
		return (1);
	}
	heap->arr[heap->size] = req;
	up_shift(heap, heap->size);
	heap->size++;
	return (0);
}

int	heap_peek(t_heap *heap, t_req *out)
{
	if (!heap || heap->size == 0)
		return (1);
	*out = heap->arr[0];
	return (0);
}
