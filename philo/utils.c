
#include "philo.h"
//del
void    ft_printlst(t_data *data)
{
    t_philo *temp;
    int i;

    temp = data->philos;
    i = 1;
    while (i <= data->number_of_philos)
    {
        printf("node: %d, last: %d, next: %d\n", temp->id, temp->last->id, temp->next->id);
        temp = temp->next;
        i++;
    }
}