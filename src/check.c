/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 18:43:26 by jibanez-          #+#    #+#             */
/*   Updated: 2021/10/25 18:43:26 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	ft_init_thread(t_table *table)
{
	int			i;
	pthread_t	*thread;

	thread = malloc(sizeof(pthread_t) * table->thinkers);
	i = -1;
	while (++i < table->thinkers)
		pthread_create(&thread[i], NULL, &parallel_life, &table->philo[i]);
	i = -1;
	while (++i < table->thinkers)
		pthread_join(thread[i], NULL);
	table->thread = thread;
}

void	ft_init_forks(t_table *table)
{
	int		i;
	int		*forks;

	i = -1;
	forks = malloc(sizeof(int) * table->thinkers);
	if (!forks)
	{
		write(1, "MALLOC ERROR\n", 14);
		return ;
	}
	while (++i < table->thinkers)
		forks[i] = TRUE;
	table->fork = forks;
}

void	ft_init_mutex(t_table *table)
{
	int				i;
	pthread_mutex_t	*mutex;

	mutex = malloc(sizeof(pthread_mutex_t) * table->thinkers);
	if (!mutex)
	{
		write(1, "MALLOC ERROR\n", 14);
		return ;
	}
	i = -1;
	while (++i < table->thinkers)
		pthread_mutex_init(&mutex[i], NULL);
	table->mutex = mutex;
}

void	ft_init_thinkers(t_table *table)
{
	int	i;

	i = -1;
	table->philo = malloc(sizeof(t_philo) * table->thinkers);
	if (table->philo == NULL)
		return ;
	while (++i < table->thinkers)
	{
		table->philo[i].id = i + 1;
		table->philo[i].t_die = table->t_die;
		table->philo[i].t_eat = table->t_eat;
		table->philo[i].last_meal_t = ft_time();
		table->philo[i].t_sleep = table->t_sleep;
		table->philo[i].n_meals = table->n_meals;
		table->philo[i].is_eating = &table->is_eating;
		table->philo[i].is_alive = &table->is_alive;
		table->philo[i].fork_left = &table->fork[i];
		table->philo[i].fork_right = &table->fork[(i + 1) % table->thinkers];
		table->philo[i].mutex_left = &table->mutex[i];
		table->philo[i].mutex_right = &table->mutex[(i + 1) % table->thinkers];
		table->philo[i].mutex_printer = &table->mutex_printer;
		table->philo[i].mutex_dead = &table->mutex_dead;
		table->philo[i].state = THINK;
		table->philo[i].start_time = table->start_time;
	}
}

int	ft_init(t_table *table, char **argv)
{
	table->thinkers = ft_atoi(argv[1]);
	table->t_die = ft_atoi(argv[2]);
	table->t_eat = ft_atoi(argv[3]);
	table->t_sleep = ft_atoi(argv[4]);
	table->is_alive = TRUE;
	table->is_eating = TRUE;
	table->start_time = ft_time();
	if (table->thinkers == 0 || table->t_die == 0 || table->t_eat == 0
		|| table->t_sleep == 0 || table->n_meals == 0)
		return (ERROR);
	ft_init_forks(table);
	ft_init_mutex(table);
	pthread_mutex_init(&table->mutex_printer, NULL);
	pthread_mutex_init(&table->mutex_dead, NULL);
	ft_init_thinkers(table);
	ft_init_thread(table);
	return (0);
}
