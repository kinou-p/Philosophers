/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apommier <apommier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 17:38:58 by apommier          #+#    #+#             */
/*   Updated: 2022/03/18 00:05:43 by apommier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_arg	*create_data(int argc, char **argv)
{
	t_arg			*data;
	pthread_mutex_t	*display;

	display = malloc(sizeof(pthread_mutex_t));
	data = malloc(sizeof(t_arg));
	if (!display || !data)
		return (0);
	pthread_mutex_init(display, 0);
	pthread_mutex_init(&data->life_check, 0);
	data->display = display;
	data->death = 1;
	data->full_philo = 0;
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->must_eat = -1;
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	return (data);
}

t_philo	*create_philo(t_arg *data)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!philo)
		return (0);
	while (i < data->nb_philo)
	{
		philo[i].last_eat = 0;
		philo[i].philo_id = i + 1;
		philo[i].data = data;
		philo[i].nb_eat = 0;
		philo[i].left_fork = malloc(sizeof(pthread_mutex_t));
		if (!philo[i].left_fork)
			return (0);
		pthread_mutex_init(philo[i].left_fork, 0);
		pthread_mutex_init(&philo[i].eat_check, 0);
		pthread_mutex_init(&philo[i].check_nb_eat, 0);
		i++;
	}
	i = -1;
	while (++i < data->nb_philo)
		philo[i].right_fork = philo[(i + 1) % data->nb_philo].left_fork;
	return (philo);
}
