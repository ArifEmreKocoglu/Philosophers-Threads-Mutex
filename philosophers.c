/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akocoglu <akocoglu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 15:47:32 by akocoglu          #+#    #+#             */
/*   Updated: 2022/09/17 17:10:28 by akocoglu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_printf(t_r *rule, int id, char *str)
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

void	check_death_two(t_p *phil, t_r *r)
{
	int	i;

	i = -1;
	while (++i < r->num_philo)
	{
		pthread_mutex_lock(&(r->lock));
		if (r->dieded)
		{
			pthread_mutex_unlock(&(r->lock));
			break ;
		}
		pthread_mutex_unlock(&(r->lock));
		pthread_mutex_lock(&(r->meal_check));
		if (calculate_time(phil[i].last_eat, start_time()) > r->time_death)
		{
			ft_printf(r, i, "is died");
			pthread_mutex_lock(&(r->lock));
			r->dieded = 1;
			pthread_mutex_unlock(&(r->lock));
		}
		pthread_mutex_unlock(&(r->meal_check));
		usleep(100);
	}
	return ;
}

void	check_death(t_r *r, t_p *phil)
{
	int	i;

	while (!(r->temp_eat))
	{
		check_death_two(phil, r);
		pthread_mutex_lock(&(r->lock));
		if (r->dieded)
		{
			pthread_mutex_unlock(&(r->lock));
			break ;
		}
		pthread_mutex_unlock(&(r->lock));
		i = 0;
		pthread_mutex_lock(&(r->lock));
		while (i < r->num_philo && phil[i].x_meal >= r->arg && r->arg != -1)
			i++;
		if (r->num_philo == i)
			r->temp_eat = 1;
		pthread_mutex_unlock(&(r->lock));
	}
}

void	exit_thread(t_r *rule, t_p *phil)
{
	int	i;

	i = -1;
	while (++i < rule->num_philo)
	{
		if (rule->num_philo == 1)
			break ;
		pthread_join(phil[i].thread_id, NULL);
	}
	i = -1;
	while (++i < rule->num_philo)
		pthread_mutex_destroy(&(rule->forks[i]));
	pthread_mutex_destroy(&(rule->writing));
	pthread_mutex_destroy(&(rule->lock));
}

int	main(int argc, char **argv)
{
	t_r	*rule;

	if (argc != 5 && argc != 6)
		return (write_error("Wrong amount of arguments"));
	if (check_args(argv))
		return (write_error("Argument Error"));
	rule = malloc(sizeof(t_r));
	rule->num_philo = ft_atoi(argv[1]);
	rule->time_death = ft_atoi(argv[2]);
	rule->time_eat = ft_atoi(argv[3]);
	rule->time_sleep = ft_atoi(argv[4]);
	rule->temp_eat = 0;
	if (rule->num_philo < 1 || rule->time_death < 60 || rule->time_eat < 60
		|| rule->time_sleep < 60 || rule->num_philo > 200)
		return (error_manager(1));
	if (argv[5])
		rule->arg = ft_atoi(argv[5]);
	else
		rule->arg = -1;
	if (mutex_init(rule))
		return (error_manager(2));
	philo_init(rule);
	if (create_thread(rule))
		return (write_error("There was an error creating the threads"));
	return (0);
}
