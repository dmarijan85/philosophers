/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarijan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 12:18:22 by dmarijan          #+#    #+#             */
/*   Updated: 2024/07/25 18:00:24 by dmarijan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* 
if philos are even, the system is already fair, synce the symmetrical nature of
their turns will prevent eachother from dying by skipping turns.
if they are odd, the time to think is gonna be (tteat * 2) - ttsleep!
this creates an issue, since if the philos synchronise incorrectly, they
will end up killing eachother by skipping someone's turn that hasn't eaten yet
*/

int	main(int argc, char **argv)
{
	t_info	info;

	if (argc == 5 || argc == 6)
	{
		info.end_simulation = false;
		parse_input(&info, argv);
		data_init(&info);
		if (!simulation_finished(&info))
			dinner_start(&info);
		clean(&info);
	}
	else
		write(2, "Wrong Input :(", 14);
}
