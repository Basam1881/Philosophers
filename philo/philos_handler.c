/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bnaji <bnaji@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 14:34:02 by bnaji             #+#    #+#             */
/*   Updated: 2022/03/03 23:03:08 by bnaji            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_forks(t_philo *philo)
{
	if (philo->n_of_philos == 1)
		return (0);
	if (*philo->is_dead)
		return (1);
	pthread_mutex_lock(philo->death_lock);
	printer(philo, 'f');
	pthread_mutex_unlock(philo->death_lock);
	if (*philo->is_dead)
		return (1);
	pthread_mutex_lock(philo->death_lock);
	printer(philo, 'f');
	pthread_mutex_unlock(philo->death_lock);
	philo->ref_time = updated_current_time(philo, 'r');
	if (ft_eat(philo))
		return (1);
	if (philo->n_times_of_eat > 0)
		philo->n_times_of_eat--;
	return (0);
}

// A normal C function that is executed as a thread 
// when its name is specified in pthread_create()
void	*philo_manager(void *vargp)
{
	t_philo	*philo;

	philo = (t_philo *)vargp;
	*philo->useless_time = updated_current_time(philo, 'r');
	philo->ref_time = *philo->useless_time;
	while (philo->n_times_of_eat && !*philo->is_dead)
	{
		// if (updated_current_time(philo, 'r') - philo->ref_time
		// 	> (unsigned long)philo->t_2_die)
		// {
		// 	if (*philo->is_dead)
		// 	{
		// 		*philo->l_fork = 1;
		// 		*philo->r_fork = 1;
		// 		return (NULL);
		// 	}
		// 	ft_die(philo);
		// 	printf("l_fork: %d\tr_fork: %d\n", *philo->l_fork, *philo->r_fork);
		// 	return (NULL);
		// }
		if (is_it_dead(philo))
			return (NULL);
		// if (philo->n_of_philos % 2 && philo->philo_id == philo->n_of_philos - 1)
		// 	*philo->l_fork = 1;
		pthread_mutex_lock(philo->locks);
		if (!(philo->philo_id % 2) && *philo->l_fork && *philo->r_fork)
		{
			*philo->l_fork = 0;
			*philo->r_fork = 0;
			take_forks(philo);
		}
		else if (philo->philo_id % 2 && *philo->l_fork && *philo->r_fork)
		{
			*philo->r_fork = 0;
			*philo->l_fork = 0;
			take_forks(philo);
		}
		pthread_mutex_unlock(philo->locks);
	}
	return (NULL);
}

void	philos_creator(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->n_of_philos)
	{
		philo_init(info, i);
		// if (!i)
		// 	*info->useless_time = updated_current_time(info->philo, 'r');
		i++;
	}
	i = 0;
	while (i < info->n_of_philos)
	{
		pthread_join(info->thread[i], NULL);
		i++;
	}
	i = 0;
	while (i < info->n_of_philos)
		pthread_mutex_destroy(info->locks[i++]);
}
