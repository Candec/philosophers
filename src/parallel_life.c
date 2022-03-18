/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parallel_life.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 16:14:34 by jibanez-          #+#    #+#             */
/*   Updated: 2022/03/10 18:59:58 by jibanez-         ###   ########.fr       */
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
	return (p);
}

void	philo_died(t_philo *philo)
{
	pthread_mutex_lock(philo->mutex_dead);
	philo->state = DEAD;
	*philo->is_alive = FALSE;
	pthread_mutex_unlock(philo->mutex_dead);
	ft_print2(philo);
}

void	ft_try_eat(t_philo *philo)
{
	if (ft_take_forks2(philo))
	{
		philo->last_meal_t = ft_time();
		philo->state = EAT;
		philo->n_meals--;
		ft_print2(philo);
		ft_hold_time(philo->last_meal_t, philo->t_eat);
		ft_free_forks(philo);
	}
}

void	ft_hold_time(uint64_t start, uint64_t action_time)
{
	// usleep(action_time);
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
	printf("%ld %d %s\n", ft_time() - philo->start_time, philo->id, action_list[philo->state]);
	pthread_mutex_unlock(philo->mutex_printer);
}

int	ft_take_forks2(t_philo *philo)
{
	ft_try_take_a_fork(philo);
	if (*philo->fork_left == FALSE || *philo->fork_right == FALSE)
	{
		ft_free_forks(philo);
		return (FALSE);
	}
	else
	{
		philo->state = FORK;
		ft_print2(philo);
		ft_print2(philo);
		return (TRUE);
	}
}

void	ft_try_take_a_fork(t_philo *philo)
{
	pthread_mutex_lock(philo->mutex_left);
	if (*philo->fork_left == TRUE)
		*philo->fork_left = FALSE;
	pthread_mutex_unlock(philo->mutex_left);
	pthread_mutex_lock(philo->mutex_right);
	if (*philo->fork_right == TRUE)
		*philo->fork_right = FALSE;
	pthread_mutex_unlock(philo->mutex_right);

}

int	philo_starved(t_philo *philo)
{
	if (ft_time() - philo->last_meal_t >= philo->t_die)
		return (TRUE);
	return (FALSE);
}

void	ft_try_sleep(t_philo *philo)
{
	philo->state = SLEEP;
	ft_print2(philo);
	ft_hold_time(ft_time(), ft_timeout2(philo, philo->t_sleep));
	// usleep(10);
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

void	ft_try_think(t_philo *philo)
{
	philo->state = THINK;
	ft_print2(philo);
	usleep(10);
	// ft_hold_time(ft_time(), ft_timeout2(philo, 0));
}