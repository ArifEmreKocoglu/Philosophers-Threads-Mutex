# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>

typedef struct  s_p
{
    int                 p_id;
    int                 p_left_id;
    int                 p_right_id;
    long long                 last_eat;
    int                 deneme;
    struct  s_r                 *rule;
    pthread_t			thread_id;
}   t_p;

typedef struct			s_r
{
	int					num_philo;
	int					time_death;
	int					time_eat;
	int					time_sleep;
    int                 dieded;
    long long           first_time;
    pthread_mutex_t		forks[250];
    pthread_mutex_t		meal_check;
	pthread_mutex_t		writing;
    t_p                 philo[250];
}						t_r;


int	main(int argc, char **argv);
int	ft_atoi(const char *nptr);
int	cntrl(int nptr);