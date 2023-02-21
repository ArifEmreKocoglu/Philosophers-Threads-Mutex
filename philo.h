/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akocoglu <akocoglu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 03:23:14 by akocoglu          #+#    #+#             */
/*   Updated: 2022/09/17 08:57:41 by akocoglu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct s_p
{
	int			p_id;
	int			p_left_id;
	int			p_right_id;
	long long	last_eat;
	int			x_meal;
	struct s_r	*rule;
	pthread_t	thread_id;
}	t_p;

typedef struct s_r
{
	int				num_philo;
	int				time_death;
	int				time_eat;
	int				time_sleep;
	int				dieded;
	long long		first_time;
	int				temp_eat;
	int				arg;
	pthread_mutex_t	forks[250];
	pthread_mutex_t	meal_check;
	pthread_mutex_t	writing;
	pthread_mutex_t	lock;
	t_p				philo[250];
}	t_r;

int			main(int argc, char **argv);
void		exit_thread(t_r *rule, t_p *phil);
void		check_death(t_r *r, t_p *phil);
void		ft_printf(t_r *rule, int id, char *str);
void		eats(t_p *phil);
void		*rule_threads(void *philos);
int			create_thread(t_r *rule);
void		philo_init(t_r *rule);
int			mutex_init(t_r *rule);
void		timing(long long time, t_r *rule);
long long	calculate_time(long long first, long long second);
long long	start_time(void);
int			ft_atoi(const char *nptr);
int			cntrl(int nptr);
void		check_death_two(t_p *phil, t_r *r);
int			write_error(char *str);
int			error_manager(int error);
int			ft_is_digit(char *str);
int			check_args(char **av);
#endif