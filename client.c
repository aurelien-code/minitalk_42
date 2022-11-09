/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumarin <aumarin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 17:36:43 by aumarin           #+#    #+#             */
/*   Updated: 2022/11/09 05:57:31 by aumarin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

char	*g_message = NULL;

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

void	send(int bit_idx, int pid)
{
	static int	*encoded_msg = 0;

	if (bit_idx == -1)
	{
		encoded_msg = malloc(sizeof(int) * (ft_strlen(g_message) * 8 + 32));
		if (!encoded_msg)
			return ;
		encode_len(encoded_msg, ft_strlen(g_message));
		encode_str(encoded_msg, g_message);
		bit_idx++;
	}
	if (bit_idx < (int)(ft_strlen(g_message) * 8 + 32) && bit_idx >= 0)
	{
		if (encoded_msg[bit_idx] == 1 && kill(pid, SIGUSR1) != 0)
			send(-2, 0);
		else if (encoded_msg[bit_idx] == 0 && kill(pid, SIGUSR2) != 0)
			send(-2, 0);
	}
	else
	{
		if (bit_idx != -2)
			ft_printf("Message sent! Length = %d\n", ft_strlen(g_message));
		free(encoded_msg);
		exit(0);
	}
}

void	handle_sig(int sig, siginfo_t *info, void *ucontext)
{
	static int	sig_count = 0;

	(void)ucontext;
	if (sig == SIGUSR1)
	{
		sig_count++;
		send(sig_count, info->si_pid);
	}
}

int	main(int argc, char **argv)
{
	struct sigaction	sa;
	int					i;

	if (argc != 3 || ft_atoi(argv[1]) <= 0)
		exit(0);
	i = 0;
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
			exit(0);
		i++;
	}
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = &handle_sig;
	sigaction(SIGUSR1, &sa, NULL);
	g_message = argv[2];
	send(-1, ft_atoi(argv[1]));
	while (1)
		pause();
	return (0);
}
