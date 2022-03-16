/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apommier <apommier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 19:10:43 by apommier          #+#    #+#             */
/*   Updated: 2022/02/20 19:10:43 by apommier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philo_eat(t_philo *philo)
{
	t_arg	*data;

	data = philo->data;
	pthread_mutex_lock(philo->left_fork);
	put_event(data, philo->philo_id, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	put_event(data, philo->philo_id, "has taken a fork");
	pthread_mutex_lock(&philo->eat_check);
	put_event(data, philo->philo_id, "is eating");
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->eat_check);
	usleep(data->time_to_eat * 1000);
	philo->nb_eat++;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	death_checker(t_philo *philo, t_arg *data)
{
	int	i;
	int	death;

	death = 1;
	while (!data->full_philo && death)
	{
		i = -1;
		while (++i < data->nb_philo - 1 && death)
		{
			pthread_mutex_lock(&philo[i].eat_check);
			if ((get_time() - philo[i].last_eat) > data->time_to_die)
			{
				put_event(data, philo[i].philo_id, "is died");
				pthread_mutex_lock(&data->life_check);
				data->death = 0;
				pthread_mutex_unlock(&data->life_check);
				death = 0;
				printf("End: one philo died\n");
				printf("time_to_die: %d TIME: %ld start: %ld philo.start: %ld last_eat: %ld condition: %ld\n", data->time_to_die ,get_time(), data->time_start , philo[i].data->time_start, get_time() - philo[i].last_eat ,get_time() - philo[i].last_eat);
			}
			pthread_mutex_unlock(&philo[i].eat_check);
			usleep(100);
			if (!death)
				break ;
			i = 0;
			while (data->must_eat != -1 && i < data->nb_philo
				&& philo[i].nb_eat >= data->must_eat)
				i++;
			if (i == data->nb_philo)
			{
				pthread_mutex_lock(&data->life_check);
				data->death = 0;
				data->full_philo = 1;
				pthread_mutex_unlock(&data->life_check);
				printf("End: all philo are full\n");
			}
		}
	}
}

void	*routine(void *tmp)
{
	t_philo	*philo;
	t_arg	*data;

	philo = (t_philo *)tmp;
	data = philo->data;
	if (philo->philo_id % 2)
		usleep(15000);
	pthread_mutex_lock(&data->life_check);
	while (philo->data->death)
	{
		pthread_mutex_unlock(&data->life_check);
		philo_eat(philo);
		put_event(data, philo->philo_id, "is sleeping");
		usleep(philo->data->time_to_sleep * 1000);
		put_event(data, philo->philo_id, "is thinking");
		pthread_mutex_lock(&data->life_check);
	}
	pthread_mutex_unlock(&data->life_check);
	return (0);
}

int	start_philo(t_philo *philo, t_arg *data)
{
	int	i;

	i = 0;
	data->time_start = get_time();
	while (i < data->nb_philo)
	{
		philo[i].last_eat = get_time();
		if (pthread_create(&philo[i].thread, 0, &routine, &philo[i]))
			return (1);
		i++;
	}
	death_checker(philo, data);
	//printf("				after d_checker\n");
	i = 0;
	while (i < data->nb_philo)
	{
		//printf("i = %d philo %d\n", i, philo[i].philo_id);
		pthread_join(philo[i++].thread, 0);
	}
//	printf("after join\n");
	exit_philo(philo, data);
	return (0);
}
