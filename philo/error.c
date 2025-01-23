/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:12:12 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/23 14:04:03 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int error_return(char *str, t_data **data)
{
    printf("Error: %s\n", str);
    free_all(data);
    return (1);
}

static void del_philo_mutex(t_philo *philo)
{
    pthread_mutex_destroy(&philo->fork);
    pthread_mutex_destroy(&philo->meal_flag);
    pthread_mutex_destroy(&philo->time_flag);
}

static void del_data_mutex(t_data *data)
{
    pthread_mutex_destroy(&data->death_flag);
    pthread_mutex_destroy(&data->print_flag);
}

void free_philos(t_philo **philos, int amount)
{
    t_philo *temp;
    t_philo *next;
    int i;
    int philo_num;

    temp = *philos;
    if (amount != -1)
        i = amount;
    else
        i = 1;
    philo_num = (*philos)->data->number_of_philos;
    while (i <= philo_num)
    {
        next = temp->next;
        del_philo_mutex(temp);
        free(temp);
        temp = next;
        i++;
    }
    *philos = NULL;
}

static void free_data(t_data **data)
{
    if (*data)
    {
        del_data_mutex(*data);
        free(*data);
        *data = NULL;
    }
}

void free_all(t_data **data)
{
    if (*data != NULL)
    {
        if ((*data)->philos)
        {
            free_philos(&(*data)->philos, -1);
            (*data)->philos = NULL;
        }
        free_data(data);
    }
}
