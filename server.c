/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumarin <aumarin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 17:36:48 by aumarin           #+#    #+#             */
/*   Updated: 2022/11/02 18:49:43 by aumarin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

char	*g_message = NULL;

int	ft_pow(int n, int pow)
{
	if (pow)
		return (n * ft_pow(n, pow - 1));
	else
		return (1);
}

int	init_message_lenght(int sig, int sig_count)
{
	int	len;

	len = 0;
	if (sig_count <= 32)
	{
		if (sig == SIGUSR1)
			len = ft_pow(2, 32 - sig_count);
		else if (sig == SIGUSR2)
			len = 0;
	}
	return (len);
}

void	decode_character(int sig, int *sig_count, int *message_len)
{
	static int	bit_count = 0;
	static int	char_count = 0;
	static int	char_val = 0;

	if (bit_count < 8)
	{
		if (sig == SIGUSR1)
			char_val += ft_pow(2, 7 - bit_count);
		bit_count++;
	}
	if (bit_count == 8)
	{
		*message_len = *message_len - 1;
		g_message[char_count] = char_val;
		char_count++;
		bit_count = 0;
		char_val = 0;
		if (*message_len == 0)
		{
			g_message[char_count] = '\0';
			ft_printf("[New message]\n%s\n", g_message);
			free(g_message);
			*sig_count = 0;
			char_count = 0;
		}
	}
}

/* 
	TODO 
		-	Check changement de pid avant la fin du recu du message ! S
			Super important pour eviter que le sereur plante alors qu'il n'a pas
			recu toute la chaine d'un client !!!
*/
void	handle_sig(int sig, siginfo_t *info, void *ucontext)
{
	static int	sig_received_count = 0;
	static int	message_len = 0;
	static int	pid_ = 0;

	(void)ucontext;
	if ((sig == SIGUSR1 || sig == SIGUSR2) && sig_received_count >= 0)
	{
		sig_received_count++;
		pid_ = info->si_pid;
		if (sig_received_count <= 32)
			message_len += init_message_lenght(sig, sig_received_count);
		if (sig_received_count == 32)
		{
			g_message = malloc(sizeof(char) * (message_len + 1));
			if (!g_message)
				return ;
		}
		if (sig_received_count > 32 && message_len > 0)
		{
			decode_character(sig, &sig_received_count, &message_len);
		}
	}
	if (info->si_pid > 0)
		kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	int					pid;
	struct sigaction	sa;

	pid = getpid();
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = &handle_sig;
	ft_printf("pid = %d\n", pid);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
	{
		pause();
	}
	return (0);
}
