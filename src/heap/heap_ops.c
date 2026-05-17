/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 15:51:46 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/17 14:49:57 by jmanani          ###   ########.fr       */
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

void	heap_push(t_heap *heap, t_req req)
{
	printf("Pushing request from coder %d with arrival time %ld and deadline\
		%ld\n", req.coder_id, req.arrival_t, req.deadline_t);
	if (!heap || heap->size >= heap->capacity)
		err_and_exit("heap_push: full or invalid heap");
	heap->arr[heap->size] = req;
	up_shift(heap, heap->size);
	heap->size++;
	printf("Heap size after push: %ld\n", heap->size);
}

t_req	heap_peek(t_heap *heap)
{
	printf("Peeking at heap. Current size: %ld\n", heap ? heap->size : -1);
	if (!heap || heap->size == 0)
		err_and_exit("heap_peek: empty or invalid heap");
	printf("Peek arrival time -> %ld\n", heap->arr[0].arrival_t);
	printf("Peek deadline time -> %ld\n", heap->arr[0].deadline_t);
	printf("Peek coder ID -> %d\n", heap->arr[0].coder_id);
	return (heap->arr[0]);
}

t_req	heap_pop(t_heap *heap)
{
	t_req	root;

	if (!heap || heap->size == 0)
		err_and_exit("heap_pop: empty or invalid heap");
	printf("heap pop called. Current size: %ld\n", heap->size);
	root = heap->arr[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->arr[0] = heap->arr[heap->size];
		down_shift(heap, 0);
	}
	printf("Popped request from coder %d with arrival time %ld and deadline\
		%ld\n", root.coder_id, root.arrival_t, root.deadline_t);
	return (root);
}
