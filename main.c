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

s_arg	*create_data(int argc, char **argv)
{
	s_arg	*data;

	data = malloc(sizeof(s_arg));
	*data->death = 1;
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->must_eat = 0;
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	//data->time_start = get_time();
	return (data);
}

void	put_event(s_philo philo, char *event)
{
	printf("%.4ldms Philo N°%d %s\n", get_time() - philo.data->time_start, philo.philo_id, event);
}

void	philo_eat(s_philo *philo_tmp)
{
	s_philo philo;

	philo = *philo_tmp;
	pthread_mutex_lock(philo.right_fork);
	put_event(philo, "has taken a fork");
	pthread_mutex_lock(philo.left_fork);
	put_event(philo, "has taken a fork");
	printf("old last eat of --%d-- = %ld\n", philo.philo_id, *philo_tmp->last_eat - philo.data->time_start);
	*philo_tmp->last_eat = get_time();
	printf("last eat of --%d-- = %ld\n", philo.philo_id, *philo_tmp->last_eat - philo.data->time_start);
	put_event(philo, "is eating");
	usleep(philo.data->time_to_eat * 1000);
	pthread_mutex_unlock(philo.right_fork);
	pthread_mutex_unlock(philo.left_fork);
	put_event(philo, "is sleeping");
	usleep(philo.data->time_to_sleep * 1000);
	put_event(philo, "is thinking");
}

void	*death_check(void *tmp)
{
	s_philo	*philo;

	philo = (s_philo *)tmp;
	while (*philo->data->death)
	{
		if (get_time() - *philo->last_eat > philo->data->time_to_die)
		{
			printf("PHILO IS DEAD last eat: %ld--  ", *philo->last_eat - philo->data->time_start);
			put_event(*philo, "PHILO IS DEAD\n\n\n--\n");
			exit(1);
		}
	}
	return (0);
}

void	*routine(void *tmp)
{
	s_philo *philo;
	pthread_t	death;

	philo = (s_philo *)tmp;

	*philo->last_eat = get_time();
	pthread_create(&death, 0, &death_check, philo);
	pthread_detach(death);
	while (1 && *philo->data->death && philo->must_eat)
	{
		philo_eat(philo);
		philo->must_eat--;
	}
	return (0); 
}

s_philo *create_philo(s_arg *data)
{
	int i;
	s_philo *philo;

	i = 0;
	philo = malloc(sizeof(s_philo) * data->nb_philo);
	if (!philo)
		return (0);
	while (i < data->nb_philo)
	{
		philo[i].philo_id = i + 1;
		philo[i].data = data;
		philo[i].must_eat = data->must_eat;
		philo[i].left_fork = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(philo[i].left_fork, 0);
		i++;
	}
	i = 0;
	while (i < data->nb_philo - 1)
	{
		philo[i].right_fork = philo[i + 1].left_fork;
		i++;
	}
	philo[i].left_fork = philo[0].right_fork;
	return (philo);
}

int	start_philo(s_philo *philo)
{
	int i;

	i = 0;
	philo->data->time_start = get_time();
	while (i < philo[i].data->nb_philo - 1)
	{
		pthread_create(&philo[i].thread, 0, &routine, &philo[i]);
		usleep(10);
		i++;
	}
	i = 0;
	while (i < philo[i].data->nb_philo - 1)
	{
		pthread_join(philo[i].thread, 0);
	}
}

int main(int argc, char **argv)
{
	s_arg *data;
	s_philo *philo;

	get_time();
	data = create_data(argc, argv);
	philo = create_philo(data);
	start_philo(philo);
}



	//put_event(*philo, "starting");
	//if ( philo->philo_id == 3)
	//	*philo->data->test = 0;
	//printf("je suis le numero %d\n", philo->philo_id);
	//printf("test = %d\n", *philo->data->test);