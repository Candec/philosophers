/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parallel_life.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 16:14:34 by jibanez-          #+#    #+#             */
/*   Updated: 2022/03/25 13:22:03 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*parallel_life(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
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
	printf("  %d n meals left: %d\n", philo->id, philo->n_meals);
	return (p);
}

void	philo_died(t_philo *philo)
{
	pthread_mutex_lock(philo->mutex_dead);
	philo->state = DEAD;
	*philo->is_alive = FALSE;
	ft_print(philo);
	pthread_mutex_unlock(philo->mutex_dead);
}

void	ft_try_eat(t_philo *philo)
{
	if (ft_try_take_a_fork(philo))
	{
		philo->last_meal_t = ft_time();
		philo->state = EAT;
		ft_print(philo);
		ft_hold_time(philo->last_meal_t, philo->t_eat);
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
	ft_hold_time(ft_time(), ft_timeout(philo, philo->t_sleep));
}

void	ft_try_think(t_philo *philo)
{
	philo->state = THINK;
	ft_print(philo);
}
