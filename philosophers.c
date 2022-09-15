/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akocoglu <akocoglu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 15:47:32 by akocoglu          #+#    #+#             */
/*   Updated: 2022/09/15 18:50:20 by akocoglu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	mutex_init(t_r *rule)
{
	int i;

	i = rule->num_philo;
	while (--i >= 0)
	{
		if (pthread_mutex_init(&(rule->forks[i]), NULL))
			return (1);
	}
	if (pthread_mutex_init(&(rule->writing), NULL))
		return (1);
	if (pthread_mutex_init(&(rule->meal_check), NULL))
		return (1);
	return (0);
}

void philo_init(t_r *rule)
{
	int i;

	i = rule->num_philo;
	while (--i >= 0)
	{
		rule->philo[i].p_id = i;
		rule->philo[i].p_left_id = i;
		rule->philo[i].p_right_id = (i + 1) % rule->num_philo;
		rule->philo[i].rule = rule;
		rule->philo[i].last_eat = 0;
		rule->philo[i].x_meal = 0;
	}
}

long long start_time(void)
{
	struct	timeval t; // mikrosaniye türünde işlemci başlanguç süresi ni tutan değişken tanımlar.

	gettimeofday(&t, NULL);							 // geçen süreyi mikrosaniye olarak atar. değişkene
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000)); // 2. parametre nanosaniyeye dönüştü.
}

long long calculate_time(long long first, long long second)
{
	return (second - first);
}

void ft_printf(t_r *rule, int id, char *str)
{
	pthread_mutex_lock(&(rule->writing));
	if (!(rule->dieded))
	{
		printf("%lli ", (start_time() - rule->first_time));
		printf("%i ", id + 1);
		printf("%s\n", str);
	}
	pthread_mutex_unlock(&(rule->writing));
}

void timing(long long time, t_r *rule)
{
	long long t;

	t = start_time();
	while (!(rule->dieded))
	{
		if	(calculate_time(t, start_time()) >= time) // süreyi geçiriyorum. start_time() artıyor.
		{
			break;
		}
		usleep(50);
	}
}

void check_death(t_r *r, t_p *phil)
{
	int i;

	while (!(r->temp_eat))
	{
		i = -1;
		while (++i < r->num_philo && !(r->dieded))
		{
			pthread_mutex_lock(&(r->meal_check));
			if (calculate_time(phil[i].last_eat, start_time()) > r->time_death)
			{
				ft_printf(r, i, "is died");
				r->dieded = 1;
			}
			pthread_mutex_unlock(&(r->meal_check));
			usleep(100);
		}
		if (r->dieded)
			break;
		i = 0;
		while(i < r->num_philo && phil[i].x_meal >= r->argv_eat && r->argv_eat != -1)
			i++;	
		if (r->num_philo == i)
			r->temp_eat = 1;	
	}
}

void eats(t_p *phil)
{
	t_r *rules;

	rules = phil->rule;
	pthread_mutex_lock(&(rules->forks[phil->p_left_id]));
	ft_printf(rules, phil->p_id, "has taken a fork");
	pthread_mutex_lock(&(rules->forks[phil->p_right_id]));
	ft_printf(rules, phil->p_id, "has taken a fork");
	pthread_mutex_lock(&(rules->meal_check));
	ft_printf(rules, phil->p_id, "is eating");
	phil->last_eat = start_time();
	pthread_mutex_unlock(&(rules->meal_check));
	timing(rules->time_eat, rules);
	(phil->x_meal)++;
	pthread_mutex_unlock(&(rules->forks[phil->p_left_id]));
	pthread_mutex_unlock(&(rules->forks[phil->p_right_id]));
}

void *rule_threads(void *philos)
{
	int	i;
	t_r	*rules;
	t_p	*phil;

	i = 0;
	phil = (t_p *)philos;
	rules = phil->rule;
	if (phil->p_id % 2)
		usleep(15000);
	while (!(rules->dieded))
	{
		eats(phil);
		if (rules->temp_eat)
			break ;
		ft_printf(rules, phil->p_id, "is sleeping");
		timing(rules->time_sleep, rules);
		ft_printf(rules, phil->p_id, "is thinking");
		i++;
	}
	return (NULL);
}

void exit_thread(t_r *rule, t_p *phil)
{
	int	i;
	i = -1;
	while (++i < rule->num_philo)
	{
		if (rule->num_philo == 1)
			break;
		pthread_join(phil[i].thread_id, NULL);
	}
		
	i = -1;
	while (++i < rule->num_philo)
		pthread_mutex_destroy(&(rule->forks[i]));
	pthread_mutex_destroy(&(rule->writing));
}

int create_thread(t_r *rule)
{
	int	i;
	t_p	*phil;

	i = 0;
	phil = rule->philo;
	rule->first_time = start_time();
	while (i < rule->num_philo)
	{
		if (pthread_create(&(phil[i].thread_id), NULL, rule_threads, &(phil[i])))
			return (1);
		phil[i].last_eat = start_time();
		i++;
	}
	check_death(rule, rule->philo);
	exit_thread(rule, phil);
	return (0);
}

int main(int argc, char **argv)
{
	t_r	*rule;

	if (argc != 5 && argc != 6)
		return (0);
	rule = malloc(sizeof(t_r));
	rule->num_philo = ft_atoi(argv[1]);
	rule->time_death = ft_atoi(argv[2]);
	rule->time_eat = ft_atoi(argv[3]);
	rule->time_sleep = ft_atoi(argv[4]);
	rule->dieded = 0;
	rule->temp_eat = 0;
	if (rule->num_philo < 1 || rule->time_death < 0 || rule->time_eat < 0
		|| rule->time_sleep < 0 || rule->num_philo > 250)
		return (0);
	if (argv[5])
		rule->argv_eat = ft_atoi(argv[5]);
	else
		rule->argv_eat = -1;
	if (mutex_init(rule))
		return (0);
	philo_init(rule);
	if (create_thread(rule))
		return (0);
}
