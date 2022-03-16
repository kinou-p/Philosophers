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

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_list
{
	int				nb_philo;
	int				full_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	long			time_start;
	int				death;
	pthread_mutex_t	life_check;
	pthread_mutex_t	*display;
}				t_arg;

typedef struct t_list
{
	int				philo_id;
	pthread_t		thread;
	t_arg			*data;
	int				nb_eat;
	long			last_eat;
	pthread_mutex_t	eat_check;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
}				t_philo;

//utils fonctions
long	ft_atoi(const char *nptr);
long	get_time(void);
void	put_event(t_arg *data, int philo_id, char *event);
t_philo	*create_philo(t_arg *data);
t_arg	*create_data(int argc, char **argv);
void	exit_philo(t_philo *philo, t_arg *data);

//philo.c
int		check_arg(int argc, char **argv);
int		start_philo(t_philo *philo, t_arg *data);
void	*routine(void *tmp);
void	death_checker(t_philo *philoo, t_arg *data);
void	philo_eat(t_philo *philo_tmp);

#endif