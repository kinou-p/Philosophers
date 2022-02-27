/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apommier <apommier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 19:10:49 by apommier          #+#    #+#             */
/*   Updated: 2022/02/20 19:10:49 by apommier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_list
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	long			time_start;
	int				death[1];
}				s_arg;

typedef struct t_list
{
	int				philo_id;
	pthread_t		thread;
	s_arg 			*data;
	int				must_eat;
	long			last_eat[1];
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;

}				s_philo;

//utils fonctions
long	ft_atoi(const char *nptr);
long	get_time(void);