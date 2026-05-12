/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:11:48 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/12 15:01:50 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

typedef pthread_mutex_t	t_mtx;

typedef struct s_args	t_args;

typedef enum e_scheduler
{
	FIFO,
	EDF
}						t_scheduler;

typedef struct s_dongle
{
	int					dongle_id;
	t_mtx				mutex;
}						t_dongle;

typedef struct s_coder
{
	int					id;
	int					compilation_done;
	bool				program_done;
	long				last_compile_start;

	pthread_t			coder_thread_id;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;

	t_args				*args;
}						t_coder;

typedef struct s_args
{
	long				n_coders;
	long				burn_time;
	long				compile_time;
	long				debug_time;
	long				refactor_time;
	long				n_compiles;
	long				cooldown_time;
	t_scheduler			scheduler;

	long				start_simulation;
	long				end_simulation;

	t_coder				*coders;
	t_dongle			*dongles;
}						t_args;

#endif