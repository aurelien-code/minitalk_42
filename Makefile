# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aumarin <aumarin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/22 17:41:33 by aumarin           #+#    #+#              #
#    Updated: 2022/10/23 12:33:38 by aumarin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all:
	gcc client.c utils/libft.a utils/libftprintf.a -o client
	gcc server.c utils/libft.a utils/libftprintf.a -o server

client:
	gcc client.c utils/libft.a utils/libftprintf.a -o client

server:
	gcc server.c utils/libft.a utils/libftprintf.a -o server

clean:
	rm -rf client
	rm -rf server
	rm -rf *.o