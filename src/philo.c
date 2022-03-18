/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 19:17:58 by jibanez-          #+#    #+#             */
/*   Updated: 2021/10/23 19:17:58 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	ft_parse(int argc, char **argv)
{
	if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[2]) <= 0
		|| ft_atoi(argv[3]) <= 0 || ft_atoi(argv[4]) <= 0)
		return (ERROR);
	if (argc == 6 && ft_atoi(argv[5]) <= 0)
		return (ERROR);
	if (ft_atoi(argv[1]) == 1)
	{
		printf("0 1 has taken a fork\n");
		usleep(ft_atoi(argv[2]));
		printf("%d 1 died\n", ft_atoi(argv[2]));
		return (EXCEPTION);
	}
	return (SUCCESS);
}

uint64_t	ft_time(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

void	ft_exit(t_table *table)
{
	int	i;

	if (table->fork)
		free(table->fork);
	if (table->philo)
		free(table->philo);
	if (table->thread)
		free(table->thread);
	i = -1;
	while (++i < table->thinkers)
	{
		if (&table->mutex[i])
			pthread_mutex_destroy(&table->mutex[i]);
	}
	if (table->mutex)
		free(table->mutex);
	if (table)
		free(table);
}

int	ft_check(t_table *table, int argc, char **argv)
{
	if (argc == 5)
	{
		table->n_meals = -1;
		ft_init(table, argv);
	}
	else if (argc == 6)
	{
		table->n_meals = ft_atoi(argv[5]);
		ft_init(table, argv);
	}
	return (SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_table	*table;
	int		ret;

	if (argc == 5 || argc == 6)
	{
		ret = ft_parse(argc, argv);
		if (ret == ERROR)
		{
			write(1, "INPUT ERROR\n", 13);
			return (ERROR);
		}
		else if (ret == EXCEPTION)
			return (SUCCESS);
		table = malloc(sizeof(t_table));
		if (!table)
		{
			write(1, "MALLOC PROBLEM\n", 16);
			return (ERROR);
		}
		if (!ft_check(table, argc, argv))
			ft_exit(table);
		return (SUCCESS);
	}
	write(1, "INVALID NUMBER OF ARGUMENTS\n", 29);
	return (ERROR);
}
