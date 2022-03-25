/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 14:37:45 by jibanez-          #+#    #+#             */
/*   Updated: 2022/03/25 13:36:24 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	philo_starved(t_philo *philo)
{
	if (ft_time() - philo->last_meal_t >= philo->t_die)
		return (TRUE);
	return (FALSE);
}

int	ft_try_take_a_fork(t_philo *philo)
{
	if (pthread_mutex_lock(philo->mutex_left))
		return (FALSE);
	if (pthread_mutex_lock(philo->mutex_right))
	{
		pthread_mutex_unlock(philo->mutex_left);
		return (FALSE);
	}
	if (*philo->fork_left == TRUE && *philo->fork_right == TRUE)
	{
		*philo->fork_left = FALSE;
		*philo->fork_right = FALSE;
		philo->state = FORK;
		ft_print(philo);
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

void	ft_free_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->mutex_left);
	pthread_mutex_lock(philo->mutex_right);
	*philo->fork_left = TRUE;
	*philo->fork_right = TRUE;
	pthread_mutex_unlock(philo->mutex_left);
	pthread_mutex_unlock(philo->mutex_right);
}

void	ft_hold_time(uint64_t start, uint64_t action_time)
{
	while ((ft_time() - start) < action_time)
		continue ;
}

void	ft_print(t_philo *philo)
{
	pthread_mutex_lock(philo->mutex_printer);
	if (philo->state == FORK)
	{
		printf("%ld %d has taken a fork\n",
			ft_time() - philo->start_time, philo->id);
		printf("%ld %d has taken a fork\n",
			ft_time() - philo->start_time, philo->id);
	}
	else if (philo->state == EAT)
		printf("%ld %d is eating\n", ft_time() - philo->start_time, philo->id);
	else if (philo->state == SLEEP)
		printf("%ld %d is sleeping\n", ft_time() - philo->start_time, philo->id);
	else if (philo->state == THINK)
		printf("%ld %d is thinking\n", ft_time() - philo->start_time, philo->id);
	else if (philo->state == DEAD)
		printf("%ld %d died\n", ft_time() - philo->start_time, philo->id);
	else
		printf("STATE ERROR");
	pthread_mutex_unlock(philo->mutex_printer);
}
