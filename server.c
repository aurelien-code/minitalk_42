/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumarin <aumarin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/22 17:36:48 by aumarin           #+#    #+#             */
/*   Updated: 2022/10/25 01:26:19 by aumarin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	handle_sig(int sig, siginfo_t *info, void *ucontext)
{
	static int	count = 8;
	static int	char_dec_value = 0;
	int			i;
	int			e;

	(void)ucontext;
	count--;
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
	else
	{
		ft_printf("%c", char_dec_value);
		count = 7;
		char_dec_value = 0;
	}
	if (info->si_pid > 0 && kill(info->si_pid, SIGUSR1))
	{
		usleep(1);
		pause();
	}
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

/*
Envoyer 8bits pour la taille du message
Une fois la taille connue, on malloc la ou on va stocker le texte
Quand le texte est recu a 100% on affiche avec printf et on free
*/
