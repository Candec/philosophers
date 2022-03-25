/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parallel_life.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 16:14:34 by jibanez-          #+#    #+#             */
/*   Updated: 2022/03/24 19:30:07 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*parallel_life(void *p)
{
	t_philo *philo;

	philo = (t_philo *)p;
	while(philo->state != DEAD && philo->n_meals != 0
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
	ft_print2(philo);
	pthread_mutex_unlock(philo->mutex_dead);
}

int	philo_starved(t_philo *philo)
{
	if (ft_time() - philo->last_meal_t >= philo->t_die)
		return (TRUE);
	return (FALSE);
}

void	ft_try_eat(t_philo *philo)
{
	if (ft_try_take_a_fork(philo))
	{
		philo->last_meal_t = ft_time();
		philo->state = EAT;
		ft_print2(philo);
		ft_hold_time(philo->last_meal_t, philo->t_eat);
		if (philo->n_meals == 0)
			ft_free_forks(philo);
	}
}

int	ft_try_take_a_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->mutex_left);
	pthread_mutex_lock(philo->mutex_right);
	if (*philo->fork_left == TRUE && *philo->fork_right == TRUE)
	{
		*philo->fork_left = FALSE;
		*philo->fork_right = FALSE;
		philo->state = FORK;
		ft_print2(philo);
		pthread_mutex_unlock(philo->mutex_left);
		pthread_mutex_unlock(philo->mutex_right);
		return (TRUE);
	}
	else
	{
		pthread_mutex_unlock(philo->mutex_left);
		pthread_mutex_unlock(philo->mutex_right);
		return (FALSE);
	}
}

void	ft_try_sleep(t_philo *philo)
{
	philo->state = SLEEP;
	philo->n_meals--;
	ft_print2(philo);
	ft_free_forks(philo);
	ft_hold_time(ft_time(), ft_timeout2(philo, philo->t_sleep));
}

void	ft_try_think(t_philo *philo)
{
	philo->state = THINK;
	ft_print2(philo);
}

void	ft_hold_time(uint64_t start, uint64_t action_time)
{
	while ((ft_time() - start) < action_time)
		continue ;
}

void	ft_print2(t_philo *philo)
{
	char	action_list[5][25] = {
		"has taken a fork",
		"is eating",
		"is sleeping",
		"is thinking",
		"died"
	};
	
	pthread_mutex_lock(philo->mutex_printer);
	if (philo->state == FORK)
	{
		printf("%ld %d %s\n", ft_time() - philo->start_time, philo->id, action_list[philo->state]);
		printf("%ld %d %s\n", ft_time() - philo->start_time, philo->id, action_list[philo->state]);
	}
	else
		printf("%ld %d %s\n", ft_time() - philo->start_time, philo->id, action_list[philo->state]);
	pthread_mutex_unlock(philo->mutex_printer);
}

uint64_t	ft_timeout2(t_philo *philo, uint64_t wait_time)
{
	uint64_t time_to_die;

	time_to_die = philo->t_die - (ft_time() - philo->last_meal_t);

	if (time_to_die < wait_time)
		return (time_to_die);
	else
		return (wait_time);
}