/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_usage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:40:26 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/30 15:13:35 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	lock_forks(t_philo *philo, t_mutex *first, t_mutex *next)
{
	pthread_mutex_lock(first);
	if (philo_msg("has taken a fork", philo) == 1)
	{
		pthread_mutex_unlock(first);
		return (1);
	}
	pthread_mutex_lock(next);
	if (philo_msg("has taken a fork", philo) == 1)
	{
		pthread_mutex_unlock(next);
		pthread_mutex_unlock(first);
		return (1);
	}
	return (0);
}

static void	unlock_forks(t_mutex *first, t_mutex *next)
{
	pthread_mutex_unlock(first);
	pthread_mutex_unlock(next);
}

int	use_forks(t_philo *philo, int check)
{
	int	res;

	if (check == 0)
	{
		if (philo->id % 2 == 0)
			res = lock_forks(philo, &philo->fork, &philo->next->fork);
		else
			res = lock_forks(philo, &philo->next->fork, &philo->fork);
		return (res);
	}
	else if (check == 1)
	{
		if (philo->id % 2 == 0)
			unlock_forks(&philo->fork, &philo->next->fork);
		else
			unlock_forks(&philo->next->fork, &philo->fork);
		return (0);
	}
	return (-1);
}
