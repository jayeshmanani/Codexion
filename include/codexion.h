/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:11:48 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/13 23:25:49 by jmanani          ###   ########.fr       */
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

// Customized Structs
typedef pthread_mutex_t			t_mtx;
typedef struct s_coding_data	t_coding_data;

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

typedef enum e_scheduler
{
	FIFO,
	EDF
}								t_scheduler;

typedef struct s_dongle
{
	int							dongle_id;
	t_mtx						dongle;
}								t_dongle;

typedef struct s_coder
{
	int							coder_id;
	int							compile_count;
	int							debug_count;
	int							refactor_count;
	bool						coder_work_done;
	long						last_compile_start;

	pthread_t					coder_thread_id;
	t_dongle					*left_dongle;
	t_dongle					*right_dongle;

	t_coding_data				*cd;
}								t_coder;

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

	long						start_coding;
	bool						end_coding;
	bool						coders_ready;

	t_coder						*coders;
	t_dongle					*dongles;
	t_mtx						cd_mutex;
}								t_coding_data;

// Other Prototypes
// utils.c
void							err_and_exit(const char *error);

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

#endif