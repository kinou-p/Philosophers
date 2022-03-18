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
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->check_nb_eat);
	philo->nb_eat++;
	pthread_mutex_unlock(&philo->check_nb_eat);
}

int	check_death(int i, t_philo *philo, t_arg *data, int *death)
{
	pthread_mutex_lock(&philo[i].eat_check);
	if ((get_time() - philo[i].last_eat) > data->time_to_die)
	{
		pthread_mutex_unlock(&philo[i].eat_check);
		put_event(data, philo[i].philo_id, "is died");
		pthread_mutex_lock(&data->life_check);
		data->death = 0;
		pthread_mutex_unlock(&data->life_check);
		*death = 0;
		printf("End: one philo died\n");
	}
	else
		pthread_mutex_unlock(&philo[i].eat_check);
	usleep(100);
	if (!(*death) || data->must_eat == -1)
		return (0);
	return (is_philo_full(philo, data));
}

void	death_checker(t_philo *philo, t_arg *data)
{
	int	i;
	int	nb_full;
	int	death;

	death = 1;
	while (!data->full_philo && death)
	{
		i = -1;
		while (++i < data->nb_philo && death)
		{
			nb_full = check_death(i, philo, data, &death);
			if (nb_full == data->nb_philo)
			{
				pthread_mutex_lock(&data->life_check);
				data->death = 0;
				death = 0;
				data->full_philo = 1;
				pthread_mutex_unlock(&data->life_check);
				printf("End: all philo are full\n");
			}
		}
	}
	printf("return death\n");
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
	i = 0;
	while (i < data->nb_philo)
	{
		printf("start join\n");
		pthread_join(philo[i++].thread, 0);
		printf("i de join= %d\n", i);
	}
	exit_philo(philo, data);
	return (0);
}
