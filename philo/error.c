/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:12:12 by aalbrech          #+#    #+#             */
/*   Updated: 2024/11/28 12:46:19 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int error_return(char *str, t_data *data)
{
    printf("Error: %s\n", str);  //can I use string.h?
    if (data != NULL)
    {
        if (data->philos != NULL)
            free(data->philos);
        free(data);
    }
    return (1);
}
