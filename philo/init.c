/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:07:40 by aalbrech          #+#    #+#             */
/*   Updated: 2024/11/28 13:41:16 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void init_data_struct(t_data *data)
{
    data->number_of_philos = 0;
    data->time_to_die = 0;
    data->time_to_eat = 0;
    data->time_to_sleep = 0;
    data->times_to_eat = 0;
    data->philos = NULL;
}

static void add_philo_back(t_philo **philos, t_philo *new)
{
    t_philo *temp;
    
    if (*philos == NULL)
    {
        *philos = new;
        return ;
    }
    temp = (*philos);
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = new;
}

static t_philo *new_philo(int id)
{
    t_philo *new;

    new = malloc(sizeof(t_philo));
    if (!new)
        return (NULL);
    new->id = id;
    new->next = NULL;
    new->last = NULL;
    new->thread = 0;
    
}

static t_philo *find_last_node(t_philo *philos, int id, t_data *data)
{
    if (id == data->number_of_philos)
        return (philos);
    while  (philos->id < id)
        philos = philos->next;
    return (philos);
}

static void make_lst_circular(t_philo *philos, t_data *data)
{
    while (philos != NULL)
    {
        philos->last = find_last_node(philos, philos->id, data);
        
    }
    
}

static int init_philo_nodes(t_data *data)
{
    t_philo *new;
    int i;

    i = 1;
    while (i <= data->number_of_philos)
    {
        new = new_philo(i);
        if (!new)
            return (1);
        add_philo_back(&data->philos, new);
        i++;
    }
    make_lst_circular(data->philos)
    
}
