/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumarin <aumarin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 17:36:43 by aumarin           #+#    #+#             */
/*   Updated: 2022/11/04 05:45:15 by aumarin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	*encode_char(char c)
{
	int	*encoded;
	int	i;

	encoded = ft_calloc(sizeof(int), 8);
	if (!encoded)
		return (NULL);
	i = 0;
	while (i < 8)
	{
		if ((c << i) & 128)
			encoded[i] = 1;
		else
			encoded[i] = 0;
		i++;
	}
	return (encoded);
}

void	send_char(char c, int pid)
{
	int	i;
	int	*encoded_char;

	i = 0;
	encoded_char = encode_char(c);
	while (i < 8)
	{
		if (encoded_char[i] == 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		pause();
		ft_printf("%d eme bit envoye\n", i + 1);
		i++;
	}
	free(encoded_char);
}

void	sig_handler(int sig)
{
	if (sig == SIGUSR1)
		ft_printf("Signal de reponse recu\n");
}

void	send_msg_len(int len, int pid)
{
	int	i;

	i = 31;
	while (i >= 0)
	{
		if ((len & (1 << i)))
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		pause();
		i--;
	}
}

int	main(int argc, char **argv)
{
	size_t	i;

	if (argc != 3 || ft_atoi(argv[1]) <= 0)
		return (1);
	ft_printf("CLIENT PID = %d\n", getpid());
	signal(SIGUSR1, sig_handler);
	send_msg_len(ft_strlen(argv[2]), ft_atoi(argv[1]));
	i = 0;
	while (i < ft_strlen(argv[2]))
	{
		send_char(argv[2][i], ft_atoi(argv[1]));
		i++;
	}
	return (0);
}
