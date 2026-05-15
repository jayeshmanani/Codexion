/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:11:48 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/15 17:11:05 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

// Default Libs
# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

// Define DEBUG_MODE
# define DEBUG_MODE 0

// Customized Structs
typedef pthread_mutex_t			t_mtx;
typedef struct s_coding_data	t_coding_data;

// Threading and Synchronization Structs
typedef enum e_pthread_ops
{
	CREATE,
	JOIN,
	INIT,
	LOCK,
	UNLOCK,
	DESTROY,
	WAIT,
	TIMEDWAIT,
	BROADCAST
}								t_pthread_ops;

typedef enum e_time_unit
{
	SEC,
	MILLISEC,
	MICROSEC
}								t_time_unit;

typedef enum e_scheduler
{
	FIFO,
	EDF
}								t_scheduler;

typedef enum e_coder_ops
{
	COMPILING,
	DEBUGGING,
	REFACTORING,
	TOOK_DONGLE_1,
	TOOK_DONGLE_2,
	BURNED_OUT
}								t_coder_ops;

// Heap Structs
typedef struct s_req
{
	int							coder_id;
	long						arrival_t;
	long						deadline_t;
}								t_req;

typedef struct s_heap
{
	t_req						*arr;
	long						size;
	long						capacity;
	t_scheduler					scheduler;
}								t_heap;

// Dongle and Coder Structs
typedef struct s_dongle
{
	int							dongle_id;
	long						next_available_t;

	pthread_cond_t				dongle_cond;
	t_mtx						dongle_state_mutex;
	t_mtx						dongle_mutex;
}								t_dongle;

typedef struct s_coder
{
	int							coder_id;
	long						compile_count;
	long						debug_count;
	long						refactor_count;
	bool						coder_work_done;
	long						last_compile_t;

	pthread_t					coder_thread_id;
	t_dongle					*left_dongle;
	t_dongle					*right_dongle;

	t_coding_data				*cd;
	t_mtx						coder_mutex;
}								t_coder;

// Input Data Struct
typedef struct s_coding_data
{
	long						n_coders;
	long						burn_time;
	long						compile_time;
	long						debug_time;
	long						refactor_time;
	long						n_compiles;
	long						cooldown_time;
	t_scheduler					scheduler;

	long						active_coders;
	long						start_coding_t;
	bool						end_coding;
	bool						coders_ready;

	t_heap						algo_heap;
	t_coder						*coders;
	t_dongle					*dongles;
	t_mtx						cd_mutex;
	t_mtx						op_mutex;

	pthread_t					analyzer;
}								t_coding_data;

// Other Prototypes
// end_utils.c
void							err_and_exit(const char *error);
void							clean_all(t_coding_data *cd);

// time_utils.c
void							abs_time_from_usec(long abs_usec,
									struct timespec *ts);
long							get_time(t_time_unit time_unit);
void							updated_usleep(t_coding_data *cd,
									long microsec);

// parsing.c
void							parse_input(t_coding_data *cd, char **argv);

// safe.c
void							*malloc_safe_create(size_t bytes);
void							mutex_safe(t_mtx *mutex, t_pthread_ops ops);
void							thread_safe(pthread_t *thread,
									t_pthread_ops ops, void *(*routine)(void *),
									void *arg);

// initialize.c
void							data_init(t_coding_data *cd);

// coding.c
void							coding_start(t_coding_data *cd);

// set_get.c
void							set_bool(t_mtx *mtx, bool *dst, bool value);
bool							get_bool(t_mtx *mtx, bool *src);
void							set_long(t_mtx *mtx, long *dst, long value);
long							get_long(t_mtx *mtx, long *src);
bool							coding_finished(t_coding_data *cd);

// sync.c
void							waiting_for_coders(t_coding_data *cd);
void							increase_long(t_mtx *mutex, long *val);
bool							all_coders_ready(t_mtx *mutex, long *threads,
									long coder_nbr);

// data_op.c
void							print_data(t_coder_ops ops, t_coder *coder,
									bool debug);

// analyser.c
void							*coding_analyser(void *args);

// dongle_sync.c
void							acquire_dongle(t_coding_data *cd,
									t_dongle *dongle);

void							release_dongle(t_coding_data *cd,
									t_dongle *dongle);

// heap Folder Prototypes
// heap_cmp.c
bool							find_preference(t_req a, t_req b,
									t_scheduler scheduler);
void							req_swap(t_req *a, t_req *b);
long							heap_parent(long i);
long							heap_left(long i);
long							heap_right(long i);

// heap_utils.c
void							heap_init(t_heap *heap, long capacity,
									t_scheduler scheduler);
void							heap_destroy(t_heap *heap);
bool							heap_is_empty(t_heap *heap);

// heap_ops.c
void							up_shift(t_heap *heap, long index);
void							down_shift(t_heap *heap, long index);
void							heap_push(t_heap *heap, t_req req);
t_req							heap_peek(t_heap *heap);
t_req							heap_pop(t_heap *heap);

#endif
