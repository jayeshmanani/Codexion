/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:11:48 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/12 14:29:41 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

typedef enum e_scheduler
{
	FIFO,
	EDF
}					t_scheduler;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
}					t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	int				compile_count;
	long			last_compile_start;

	t_dongle		*left;
	t_dongle		*right;

}					t_coder;

#endif