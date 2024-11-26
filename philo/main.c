/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 13:23:56 by aalbrech          #+#    #+#             */
/*   Updated: 2024/11/26 16:44:50 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int ac, char **av)
{
    t_philo *philo;

    philo = NULL;
    if (ac == 5 || ac == 6)
    {
        philo = malloc(sizeof(t_philo));
        if (!philo)
            return (error_return("Malloc problem in main", philo));
        if (init_philo_node(philo) == 1)
            return (error_return("Malloc problem in init_philo_node", philo));
        if (handle_input(ac, av, philo->data) == 1)
            return (error_return("Input is invalid", philo));
    }
    else
    {
        error_return("Wrong amount of arguments", philo);
        return (0);
    }
    free(philo->data);
    free(philo);
    return (0);
}
