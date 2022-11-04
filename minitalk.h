/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumarin <aumarin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 15:46:40 by aumarin           #+#    #+#             */
/*   Updated: 2022/11/04 01:40:18 by aumarin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <sys/types.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
//# include "./utils/libft/libft.h"
# include "./utils/ft_printf/ft_printf.h"


int		ft_atoi(const char *str);
int		ft_printf(const char *str, ...);
size_t	ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
int		ft_pow(int n, int pow);
void	*ft_calloc(size_t count, size_t size);

#endif