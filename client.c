/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumarin <aumarin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 17:36:43 by aumarin           #+#    #+#             */
/*   Updated: 2022/11/09 03:31:35 by aumarin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	encode_str(int *ecd_msg, char *str)
{
	int	i;
	int	c;
	int	e;

	e = 32;
	while (*str)
	{
		i = 8;
		c = *str++;
		while (i--)
		{
			if (c >> i & 1)
				ecd_msg[e] = 1;
			else
				ecd_msg[e] = 0;
			e++;
		}
	}
}

void	encode_len(int *ecd_msg, int len)
{
	int	i;
	int	e;

	e = 0;
	i = 31;
	while (i >= 0)
	{
		if ((len & (1 << i)))
			ecd_msg[e] = 1;
		else
			ecd_msg[e] = 0;
		e++;
		i--;
	}
}

void	handle_sig(int sig)
{
	static int	sig_count = 0;

	if (sig == SIGUSR1)
	{
		sig_count++;
		ft_putstr_fd("Signal recu #", 1);
		ft_putnbr_fd(sig_count, 1);
		ft_putstr_fd("\n", 1);
	}
	return ;
}

void	is_args_valid(int argc, char **argv)
{
	int	i;

	if (argc != 3 || ft_atoi(argv[1]) <= 0)
		exit(0);
	i = -1;
	while (argv[1][++i])
	{
		if (!ft_isdigit(argv[1][i]))
			exit(0);
	}
	return ;
}

int	main(int argc, char **argv)
{
	size_t	i;
	int		*encoded_msg;

	is_args_valid(argc, argv);
	signal(SIGUSR1, &handle_sig);
	encoded_msg = malloc(sizeof(int) * ((ft_strlen(argv[2]) * 8) + 32));
	if (!encoded_msg)
		return (1);
	encode_len(encoded_msg, ft_strlen(argv[2]));
	encode_str(encoded_msg, argv[2]);
	i = 0;
	while (i <= (ft_strlen(argv[2]) * 8 + 32))
	{
		if (encoded_msg[i] == 1 && kill(ft_atoi(argv[1]), SIGUSR1) == 0)
			i++;
		else if (encoded_msg[i] == 0 && kill(ft_atoi(argv[1]), SIGUSR2) == 0)
			i++;
		else
			break ;
		pause();
	}
	free(encoded_msg);
	return (0);
}
