/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 19:31:44 by jibanez-          #+#    #+#             */
/*   Updated: 2022/03/28 12:04:14 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	ft_isdigit(int c)
{
	return ((c >= '0' && c <= '9'));
}

static int	ft_isspace(char a)
{
	if ((a >= 9 && a <= 13) || a == 32)
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	long long int	i;
	long long int	result;
	int				negative;

	result = 0;
	negative = 1;
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			negative = -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * negative);
}

uint64_t	ft_timeout(t_philo *philo, uint64_t wait_time)
{
	uint64_t	time_to_die;

	time_to_die = philo->t_die - (ft_time() - philo->last_meal_t);
	if (time_to_die < wait_time)
		return (time_to_die);
	else
		return (wait_time);
}

int	ft_lock(t_philo *philo)
{
	if (philo->id % 2)
	{
		if (pthread_mutex_lock(philo->mutex_left))
			return (FALSE);
		if (pthread_mutex_lock(philo->mutex_right))
		{
			pthread_mutex_unlock(philo->mutex_left);
			return (FALSE);
		}
	}
	else
	{
		if (pthread_mutex_lock(philo->mutex_right))
			return (FALSE);
		if (pthread_mutex_lock(philo->mutex_left))
		{
			pthread_mutex_unlock(philo->mutex_right);
			return (FALSE);
		}
	}
	return (TRUE);
}
