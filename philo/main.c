/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 13:23:56 by aalbrech          #+#    #+#             */
/*   Updated: 2024/11/28 12:45:38 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int ac, char **av)
{
    t_data *data;

    data = NULL;
    if (ac == 5 || ac == 6)
    {
        data = malloc(sizeof(t_data));
        if (!data)
            return (error_return("Malloc problem in main", data));
        init_data_struct(data);
        if (handle_input(ac, av, data) == 1)
            return (error_return("Input is invalid", data));
    }
    else
    {
        error_return("Wrong amount of arguments", data);
        return (0);
    }

    error_return("Success", data);
    return (0);
}
