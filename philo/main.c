/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 13:23:56 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/23 14:57:18 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int ac, char **av)
{
    t_data *data;

    data = NULL;
    if (ac != 5 && ac != 6)
        return (error_return("Wrong amount of arguments", &data));
    data = malloc(sizeof(t_data));
    if (!data)
        return (error_return("Malloc problem in main", &data));
    if (init_data_struct(data) == 1)
    {
        free(data);
        return (1);
    }
    if (handle_input(ac, av, data) == 1)
        return (error_return("Input is invalid", &data));
    if (init_philo_nodes(data) == 1)
        return (error_return("Malloc problem in init_philo_nodes", &data));
    if (philo_time(data) == 1)
        return (1);
    free_all(&data);
    return (0);
}

/*
- error check all functions, also those which are not yours
*/
