/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:07:40 by aalbrech          #+#    #+#             */
/*   Updated: 2024/11/29 15:00:12 by aalbrech         ###   ########.fr       */
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
    data->real_start_time = 0;
    data->start_time = 0;
    data->manager_thread = 0;
    data->death = 0;
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

static t_philo *new_philo(int id, t_data *data)
{
    t_philo *new;

    new = malloc(sizeof(t_philo));
    if (!new)
        return (NULL);
    new->id = id;
    new->next = NULL;
    new->last = NULL;
    new->thread = 0;
    new->data = data;
    new->eat_time = 0;
    new->started_eating = 0;
    return (new);
}

static void make_lst_circular(t_philo **philos)
{
    t_philo *head;
    t_philo *prev;

    head = *philos;
    prev = NULL;
    while (*philos != NULL)
    {
        if (prev != NULL)
            (*philos)->last = prev;
        prev = *philos;
        *philos = (*philos)->next;
    }
    prev->next = head;
    head->last = prev;
    *philos = head;
}

int init_philo_nodes(t_data *data)
{
    t_philo *new;
    int i;

    i = 1;
    while (i <= data->number_of_philos)
    {
        new = new_philo(i, data);
        if (!new)
            return (1);
        add_philo_back(&data->philos, new);
        i++;
    }
    make_lst_circular(&data->philos);
    return (0);    
}
