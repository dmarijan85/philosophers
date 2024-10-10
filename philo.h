/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarijan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 12:19:21 by dmarijan          #+#    #+#             */
/*   Updated: 2024/07/25 12:47:11 by dmarijan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

typedef enum e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	JOIN,
	CREATE,
	DETACH,
}			t_opcode;

typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED,
}			t_philo_status;

typedef enum e_timecode
{
	SECONDS,
	MILLISECONDS,
	MICROSECONDS,
}			t_timecode;

typedef pthread_mutex_t	t_mtx;

typedef struct s_info	t_info;

typedef struct s_fork
{
	t_mtx	fork;
	int		fork_id;
}				t_fork;

typedef struct s_philo
{
	int			id;				//whats my philo id
	long		meals_counter;	//how many meals consumed
	bool		full;			//is my philosopher satisfied?
	long		lastmealtime;	//when is the last time my philosopher ate?
	t_fork		*first_fork;
	t_fork		*second_fork;
	pthread_t	thread_id;		//my philo's thread id
	t_info		*info;
	t_mtx		philo_mutex;	//necessary for the monitor to not race
}				t_philo;

typedef struct s_info
{
	long		max_philos;
	long		ttdie;
	long		tteat;
	long		ttsleep;
	long		nbr_limit_meals;	//limit of meals to eat, -1 if no limit
	long		start_time;			//timestamp of simulation start
	bool		end_simulation;		//something finished the simulation
	bool		all_threads_ready;	//used to synchronise the philos
	t_fork		*forks;
	long		threadsrunningnbr;
	long		philosfullnbr;
	pthread_t	monitor;
	t_mtx		table_mutex;
	t_mtx		write_mutex;
	t_philo		*philos;
}				t_info;

void	errexit(const char *error, t_info *info);
void	parse_input(t_info *info, char **argv);
void	*safemalloc(size_t bytes, t_info *info);
void	safemutexer(t_mtx *mutex, t_opcode opcode, t_info *info);
void	safethreader(pthread_t *thread, t_opcode opc, t_info *info);
void	setlong(t_mtx *mutex, long *dest, long value, t_info *info);
long	getlong(t_mtx *mutex, long *dest, t_info *info);
void	setbool(t_mtx *mutex, bool *dest, bool value, t_info *info);
bool	getbool(t_mtx *mutex, bool *dest, t_info *info);
bool	simulation_finished(t_info *info);
void	wait_threads(t_info *info);
long	gettime(t_timecode timecode, t_info *info);
void	ft_usleep(long usec, t_info *info);
void	write_status(t_philo_status status, t_philo *philo, t_info *info);
void	handle_thread(int status, t_opcode opcode, t_info *info);
void	data_init(t_info *info);
void	dinner_start(t_info *info);
bool	allthreadsrunning(t_mtx *mtx, long *nbrrdy, long phnbr, t_info *info);
void	*increase_long(t_mtx *mutex, long *value, t_info *info);
void	*monitor_dinner(void *data);
void	clean(t_info *info);
void	thinking(t_philo *philo, t_info *info, bool flag);
void	desynchro_philos(t_philo *philo);
