/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_tools.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akocoglu <akocoglu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 03:05:18 by akocoglu          #+#    #+#             */
/*   Updated: 2022/09/17 16:40:04 by akocoglu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	mutex_init(t_r *rule)
{
	int	i;

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
	if (pthread_mutex_init(&(rule->lock), NULL))
		return (1);
	return (0);
}

void	philo_init(t_r *rule)
{
	int	i;

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

int	create_thread(t_r *rule)
{
	int	i;
	t_p	*ph;

	i = 0;
	ph = rule->philo;
	pthread_mutex_lock(&(rule->lock));
	rule->first_time = start_time();
	pthread_mutex_unlock(&(rule->lock));
	while (i < rule->num_philo)
	{
		if (pthread_create(&(ph[i].thread_id), NULL, rule_threads, &(ph[i])))
			return (1);
		pthread_mutex_lock(&(rule->lock));
		ph[i].last_eat = start_time();
		pthread_mutex_unlock(&(rule->lock));
		i++;
	}
	check_death(rule, rule->philo);
	exit_thread(rule, ph);
	return (0);
}

void	*rule_threads(void *philos)
{
	t_r	*rules;
	t_p	*phil;

	phil = (t_p *)philos;
	rules = phil->rule;
	if (phil->p_id % 2)
		usleep(15000);
	while (!(rules->dieded))
	{
		pthread_mutex_lock(&(rules->lock));
		if (rules->temp_eat)
		{
			pthread_mutex_unlock(&(rules->lock));
			break ;
		}
		pthread_mutex_unlock(&(rules->lock));
		eats(phil);
		ft_printf(rules, phil->p_id, "is sleeping");
		timing(rules->time_sleep, rules);
		ft_printf(rules, phil->p_id, "is thinking");
	}
	return (NULL);
}

void	eats(t_p *phil)
{
	t_r	*rules;

	rules = phil->rule;
	pthread_mutex_lock(&(rules->forks[phil->p_left_id]));
	ft_printf(rules, phil->p_id, "has taken a fork");
	pthread_mutex_lock(&(rules->forks[phil->p_right_id]));
	ft_printf(rules, phil->p_id, "has taken a fork");
	pthread_mutex_lock(&(rules->meal_check));
	pthread_mutex_lock(&(rules->lock));
	if (!(rules->temp_eat))
		ft_printf(rules, phil->p_id, "is eating");
	pthread_mutex_unlock(&(rules->lock));
	pthread_mutex_lock(&(rules->lock));
	phil->last_eat = start_time();
	pthread_mutex_unlock(&(rules->lock));
	pthread_mutex_lock(&(rules->lock));
	phil->x_meal++;
	pthread_mutex_unlock(&(rules->lock));
	pthread_mutex_unlock(&(rules->meal_check));
	pthread_mutex_unlock(&(rules->forks[phil->p_left_id]));
	timing(rules->time_eat, rules);
	pthread_mutex_unlock(&(rules->forks[phil->p_right_id]));
}
