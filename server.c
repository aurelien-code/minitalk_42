/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumarin <aumarin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 17:36:48 by aumarin           #+#    #+#             */
/*   Updated: 2022/10/27 04:06:23 by aumarin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

char	*g_message = NULL;

int	decode_save(int count, int sig)
{
	int			char_dec_value;
	int			i;
	int			e;

	char_dec_value = 0;
	if (count >= 0)
	{
		if (sig == SIGUSR1)
		{
			i = -1;
			e = 1;
			while (++i < count)
				e *= 2;
			char_dec_value += e;
		}
	}
	return (char_dec_value);
}

int	init_message_length(int sig, int i, int len)
{
	if (sig == SIGUSR1)
		len += 1 << i;
	else if (sig == SIGUSR2)
		len += 0 << i;
	if (i == 0)
	{
		g_message = malloc(sizeof(char) * (len + 1));
		if (!g_message)
			return (-1);
	}
	i--;
	return (len);
}

void	add_char_in_message(int *count, int *char_dec_value, int *e, int sig)
{
	if (--*count >= 0)
	{
		if (sig == SIGUSR1)
			*char_dec_value += decode_save(*count, sig);
	}
	else
	{
		if (*char_dec_value != 0)
			g_message[++*e] = *char_dec_value;
		*count = 7;
		*char_dec_value = 0;
	}
}

void	handle_sig(int sig, siginfo_t *info, void *ucontext)
{
	static int	count = 8;
	static int	char_dec_value = 0;
	static int	len = 0;
	static int	i = 32;
	static int	e = -1;

	(void)ucontext;
	if (--i >= 0)
		len = init_message_length(sig, i, len);
	else
	{
		add_char_in_message(&count, &char_dec_value, &e, sig);
		if (count == 0 && char_dec_value == 0)
		{
			g_message[len] = '\0';
			len = 0;
			e = -1;
			i = 32;
			ft_printf("New message -> %s\n", g_message);
			free(g_message);
		}
	}
	if (info->si_pid > 0 && kill(info->si_pid, SIGUSR1))
		pause();
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
		;
	return (0);
}
