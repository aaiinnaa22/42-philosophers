/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 13:24:34 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/30 15:25:50 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h> 
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

# define YELLOW "\033[0;33m"
# define RED "\033[0;31m"
# define RESET "\033[0m"

typedef pthread_mutex_t	t_mutex;

typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	long				eat_time;
	pthread_t			thread;
	pthread_mutex_t		fork;
	int					meal_count;
	pthread_mutex_t		meal_flag;
	pthread_mutex_t		time_flag;
	struct s_philo		*next;
	t_data				*data;
}	t_philo;

struct	s_data
{
	int					number_of_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					times_to_eat;
	int					death;
	pthread_mutex_t		death_flag;
	long				start;
	pthread_t			manager_thread;
	pthread_mutex_t		print_flag;
	t_philo				*philos;
};

int		insert_data(int ac, char **av, t_data *data);
int		set_data_struct(int ac, char **av, t_data **data);
int		init_philo_nodes(t_data **data);
int		error_return(char *str, t_data **data, int check);
void	free_all(t_data **data);
void	free_data(t_data **data);
void	del_philo_mutex(t_philo *philo);
int		threading(t_data *data);
int		death_check(t_data *data);
int		philo_msg(char *s, t_philo *philo);
int		philo_thinks(t_philo *philo);
int		philo_eats(t_philo *philo);
int		philo_sleeps(t_philo *philo);
void	syncro_start(long start_time);
void	*death_manager(void *content);
void	*philo_doing(void *content);
void	*one_philo(void *content);
long	time_is(void);
int		use_forks(t_philo *philo, int check);

#endif
