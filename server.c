/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumarin <aumarin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 17:36:48 by aumarin           #+#    #+#             */
/*   Updated: 2022/11/09 05:57:16 by aumarin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

char	*g_message = NULL;

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

void	decode_character(int sig, int *message_len)
{
	static int	bit_count = 0;
	static int	char_count = 0;
	static int	char_val = 0;

	if (sig == -1)
	{
		bit_count = 0;
		char_val = 0;
		char_count = 0;
		return ;
	}
	if (bit_count < 8 && ++bit_count && sig == SIGUSR1)
			char_val += ft_pow(2, 8 - bit_count);
	if (bit_count == 8 && ++char_count)
	{
		*message_len = *message_len - 1;
		g_message[char_count - 1] = char_val;
		bit_count = 0;
		char_val = 0;
		if (*message_len == 0)
		{
			ft_printf("Msg length = %d\n%s\n", ft_strlen(g_message), g_message);
			free(g_message);
		}
	}
}

void	reset_server(int *sig_count, int *msg_len, int *pid_c, siginfo_t *info)
{
	*sig_count = 0;
	if (*msg_len > 0)
		free(g_message);
	*msg_len = 0;
	*pid_c = info->si_pid;
	*sig_count = 0;
	decode_character(-1, msg_len);
}

void	handle_sig(int sig, siginfo_t *info, void *ucontext)
{
	static int	sig_count = 0;
	static int	message_len = 0;
	static int	pid_client = 0;

	if (pid_client != info->si_pid)
		reset_server(&sig_count, &message_len, &pid_client, info);
	(void)ucontext;
	if ((sig == SIGUSR1 || sig == SIGUSR2))
	{
		sig_count++;
		if (pid_client == 0 && sig_count >= 1)
			pid_client = info->si_pid;
		if (sig_count <= 32)
			message_len += init_message_lenght(sig, sig_count);
		if (sig_count == 32 && message_len > 0)
		{
			g_message = ft_calloc(sizeof(char), message_len + 1);
			if (!g_message)
				return ;
		}
		if (sig_count > 32 && message_len > 0)
			decode_character(sig, &message_len);
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
