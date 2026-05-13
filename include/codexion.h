/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:11:48 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/13 16:36:54 by jmanani          ###   ########.fr       */
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

typedef enum e_mutex_ops
{
	CREATE,
	JOIN,
	INIT,
	LOCK,
	UNLOCK,
	DESTROY,
}								t_mutex_ops;

// typedef enum e_cond_ops
// {
// 	INIT,
// 	WAIT,
// 	TIMEDWAIT,
// 	BROADCAST,
// 	DESTROY
// }								t_cond_ops;

typedef enum e_scheduler
{
	FIFO,
	EDF
}								t_scheduler;

typedef struct s_dongle
{
	int							dongle_id;
	t_mtx						mutex;
}								t_dongle;

typedef struct s_coder
{
	int							id;
	int							compilation_done;
	bool						program_done;
	long						last_compile_start;

	pthread_t					coder_thread_id;
	t_dongle					*left_dongle;
	t_dongle					*right_dongle;

	t_coding_data				*args;
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

	long						start_simulation;
	bool						end_simulation;

	t_coder						*coders;
	t_dongle					*dongles;
}								t_coding_data;

// Other Prototypes from different files
void							error_exit(const char *error);
void							parse_input(t_coding_data *coding_data,
									char **argv);
void							*malloc_safe_create(size_t bytes);
void							*mutex_safe(t_mtx *mutex, t_mutex_ops ops);

#endif