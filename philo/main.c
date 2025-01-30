/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 13:23:56 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/30 14:21:42 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_data	*data;

	data = NULL;
	if (ac != 5 && ac != 6)
		return (error_return("Wrong amount of arguments", &data, 0));
	data = malloc(sizeof(t_data));
	if (!data)
		return (error_return("Malloc problem in main", &data, 0));
	if (set_data_struct(ac, av, &data) == 1)
		return (1);
	if (init_philo_nodes(&data) == 1)
		return (1);
	if (threading(data) == 1)
		return (error_return("Couldn't create thread", &data, 1));
	free_all(&data);
	return (0);
}
