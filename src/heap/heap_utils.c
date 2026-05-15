/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 16:01:46 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/15 16:58:53 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_init(t_heap *heap, long capacity, t_scheduler scheduler)
{
	if (!heap || capacity <= 0)
		err_and_exit("heap_init: invalid args");
	heap->arr = malloc_safe_create(sizeof(t_req) * capacity);
	heap->size = 0;
	heap->capacity = capacity;
	heap->scheduler = scheduler;
}

bool	heap_is_empty(t_heap *heap)
{
	if (!heap)
		return (true);
	return (heap->size == 0);
}

// void	heap_destroy(t_heap *heap)
// {
// 	if (!heap)
// 		return ;
// 	free(heap->arr);
// 	heap->arr = NULL;
// 	heap->size = 0;
// 	heap->capacity = 0;
// Will check later if need to free separately or will handle from comming
// clean_all function
// }
