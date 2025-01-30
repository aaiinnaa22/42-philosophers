/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:09:33 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/30 11:42:32 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int end_threads(t_data *data, int thread_count)
{
    t_philo *temp;
    int i;
    int num;
    
    temp = data->philos;
    i = 1;
    if (thread_count == -1)
        num = data->number_of_philos;
    else
        num = thread_count;
    while (i <= num)
    {
        pthread_join(temp->thread, NULL);
        temp = temp->next;
        i++;
    }
    pthread_join(data->manager_thread, NULL);
    return (1);
}

static int make_threads(t_data *data)
{
    t_philo *temp;
    int i;

    data->start_time = time_is() + (data->number_of_philos * 20); 
    temp = data->philos;
    i = 1;
    if (data->number_of_philos == 1)
    {
        if (pthread_create(&temp->thread, NULL, one_philo, temp) != 0)
            return (1);
        pthread_join(temp->thread, NULL);
        return (2);
    }
    if (pthread_create(&data->manager_thread, NULL, death_manager, data) != 0)
        return (1);
    while (i <= data->number_of_philos)
    {
        if (pthread_create(&temp->thread, NULL, philo_doing, temp) != 0)
            return (end_threads(data, i - 1));
        temp = temp->next;
        i++;
    }
    return (0);
}

int    threading(t_data *data)
{
    int res;
    
    res = make_threads(data);
    if (res != 0)
        return (res);
    end_threads(data, -1);
    return (0);
}
