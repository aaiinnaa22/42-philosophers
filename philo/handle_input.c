/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 13:44:28 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/30 14:20:14 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	input_is_num(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

static int	ft_atoi(char *str)
{
	int			i;
	long int	res;

	if (input_is_num(str) == 1)
		return (-1);
	res = 0;
	i = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			return (-1);
		i++;
	}
	while (str[i] != '\0')
	{
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	if ((res) > 2147483647)
		return (-1);
	return (res);
}

int	insert_data(int ac, char **av, t_data *data)
{
	data->number_of_philos = ft_atoi(av[1]);
	if (data->number_of_philos < 0)
		return (1);
	data->time_to_die = ft_atoi(av[2]);
	if (data->time_to_die < 0)
		return (1);
	data->time_to_eat = ft_atoi(av[3]);
	if (data->time_to_die < 0)
		return (1);
	data->time_to_sleep = ft_atoi(av[4]);
	if (data->time_to_sleep < 0)
		return (1);
	if (ac == 6)
	{
		data->times_to_eat = ft_atoi(av[5]);
		if (data->times_to_eat < 0)
			return (1);
	}
	return (0);
}
