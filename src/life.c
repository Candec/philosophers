/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 16:14:34 by jibanez-          #+#    #+#             */
/*   Updated: 2022/05/21 20:04:36 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*parallel_life(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	philo->state = THINK;
	while (philo->state != DEAD && philo->n_meals != 0
		&& *philo->is_alive == TRUE)
	{
		if (philo_starved(philo))
			philo_died(philo);
		else if (philo->state == THINK)
			ft_try_eat(philo);
		else if (philo->state == EAT)
			ft_try_sleep(philo);
		else if (philo->state == SLEEP)
			ft_try_think(philo);
	}
	return (p);
}

void	philo_died(t_philo *philo)
{
	if (!pthread_mutex_lock(philo->mutex_dead))
	{
		if (*philo->is_alive == TRUE)
		{
			philo->state = DEAD;
			*philo->is_alive = FALSE;
			ft_print(philo);
		}
		pthread_mutex_unlock(philo->mutex_dead);
	}
}

void	ft_try_eat(t_philo *philo)
{
	if (ft_try_take_a_fork(philo))
	{
		philo->last_meal_t = ft_time(philo->start_time);
		philo->state = EAT;
		ft_print(philo);
		ft_hold_time(philo->last_meal_t, philo->t_eat, philo);
		if (philo->n_meals == 0)
			ft_free_forks(philo);
	}
}

void	ft_try_sleep(t_philo *philo)
{
	philo->state = SLEEP;
	philo->n_meals--;
	ft_print(philo);
	ft_free_forks(philo);
	ft_hold_time(ft_time(philo->start_time), philo->t_sleep, philo);
}

void	ft_try_think(t_philo *philo)
{
	philo->state = THINK;
	ft_print(philo);
}
