/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 19:17:51 by jibanez-          #+#    #+#             */
/*   Updated: 2021/10/23 19:17:51 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <stdbool.h>
# include <stdint.h>

# define ONE_SEC 1000000
# define ERROR -1
# define SUCCESS 0
# define EXCEPTION 7
# define FAILURE 1
# define FALSE 0
# define TRUE 1

enum
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DEAD
};

typedef struct s_philo
{
	int				id;
	int				state;
	uint64_t		t_die;
	uint64_t		t_eat;
	uint64_t		t_sleep;
	int				n_meals;
	int				*is_alive;
	int				*is_eating;
	int				*fork_right;
	int				*fork_left;
	uint64_t		start_time;
	uint64_t		start_eating;
	uint64_t		last_meal_t;
	pthread_mutex_t	*mutex_printer;
	pthread_mutex_t	*mutex_dead;
	pthread_mutex_t	*mutex_right;
	pthread_mutex_t	*mutex_left;
}					t_philo;

typedef struct s_table
{
	int				thinkers;
	uint64_t		t_die;
	uint64_t		t_eat;
	uint64_t		t_sleep;
	uint64_t		t_think;
	int				n_meals;
	int				is_alive;
	int				*fork;
	int				is_eating;
	uint64_t		start_time;
	t_philo			*philo;
	pthread_t		*thread;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	mutex_printer;
	pthread_mutex_t	mutex_dead;
}					t_table;

/*
** philo.c
*/
int			ft_parse(int argc, char **argv);
uint64_t	ft_time(void);
void		ft_print(t_philo *philo);
int			ft_check(t_table *table, int argc, char **argv);

/*
** check.c
*/
void		ft_init_forks(t_table *table);
void		ft_init_mutex(t_table *table);
void		ft_init_thinkers(t_table *table);
void		ft_init_thread(t_table *table);
int			ft_init(t_table *table, char **argv);

/*
**	utils.c
*/
void		ft_print(t_philo *philo);
void		ft_hold_time(uint64_t start, uint64_t action_time);
void		ft_free_forks(t_philo *philo);
int			philo_starved(t_philo *philo);
int			ft_try_take_a_fork(t_philo *philo);

/*
** utils_2.c
*/
int			ft_atoi(const char *str);
uint64_t	ft_timeout(t_philo *philo, uint64_t wait_time);

/*
**	Paralel_life.c
*/
void		*parallel_life(void *p);
void		ft_try_eat(t_philo *philo);
void		ft_try_sleep(t_philo *philo);
void		ft_try_think(t_philo *philo);
void		philo_died(t_philo *philo);

#endif