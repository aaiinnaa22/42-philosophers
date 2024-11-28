/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 13:24:34 by aalbrech          #+#    #+#             */
/*   Updated: 2024/11/28 20:38:42 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h> //printf
# include <stdlib.h> //malloc free
# include <pthread.h> //var pthread_t
# include <unistd.h> //usleep

typedef struct s_data   t_data; //I use t_data in t_philo before the computer knows about the struct

typedef struct s_philo
{
    int id;
    pthread_t thread;
    pthread_mutex_t fork;
    struct s_philo *last;
    struct s_philo *next;
    t_data  *data;
} t_philo;

struct s_data
{
    int number_of_philos;
    int time_to_die; //time philo will die if doesnt eat
    int time_to_eat; //time it takes for philo to eat
    int time_to_sleep; //time it takes for philo to sleep
    int times_to_eat; //amount of times each philo has to eat !optional
    t_philo *philos;
};

//handle input
int handle_input(int ac, char **av, t_data *data);

//init
void init_data_struct(t_data *data);
int init_philo_nodes(t_data *data);

//error
int error_return(char *str, t_data *data);

//philo time
void    philo_time(t_data *data);
void    eat(t_philo *philo);
void    to_sleep(t_philo *philo);
void    think(t_philo *philo);

//del
void    ft_printlst(t_data *data);


#endif