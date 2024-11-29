//Stdheader

#include "philo.h"

long    init_time_is()
{
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long    time_is(long real_start_time)
{
    struct timeval tv;
    long    current_time;

    gettimeofday(&tv, NULL);
    current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return (current_time - real_start_time);
}
