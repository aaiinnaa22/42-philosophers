/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:12:12 by aalbrech          #+#    #+#             */
/*   Updated: 2024/11/26 16:40:04 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int error_return(char *str, t_philo *philo)
{
    printf("Error: %s\n", str);  //can I use string.h?
    if (philo != NULL)
    {
        if (philo->data != NULL)
            free(philo->data);
        free(philo);
    }
    return (1);
}
