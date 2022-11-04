# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aumarin <aumarin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/26 13:20:05 by aumarin           #+#    #+#              #
#    Updated: 2022/11/04 01:42:24 by aumarin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SERVER_NAME = server
CLIENT_NAME = client

SERVER_SRC = server.c
SERVER_OBJ = $(SERVER_SRC:.c=.o)
CLIENT_SRC = client.c
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
C_FLAGS = -Wall -Wextra -Werror
CC = gcc
LIBFT_PATH = ./utils/libft/
FT_PRINTF_PATH = ./utils/ft_printf/

all: $(SERVER_NAME) $(CLIENT_NAME)

.c.o:
	@echo "\033[0;33mcompiling... \033[0;37m"
	@gcc $(C_FLAGS)  -I . -c $< -o $(<:.c=.o)

$(SERVER_NAME): $(SERVER_OBJ)
	@make -C $(LIBFT_PATH)
	@make -C $(FT_PRINTF_PATH)
	@echo "\033[0;33mlinking server... \033[0;37m"
	@$(CC)  $(C_FLAGS) -o $@ $^ $(FT_PRINTF_PATH)/libftprintf.a $(LIBFT_PATH)/libft.a

$(CLIENT_NAME): $(CLIENT_OBJ)
	@echo "\033[0;33mlinking client... \033[0;37m"	
	@make -C $(LIBFT_PATH)
	@make -C $(FT_PRINTF_PATH)
	@$(CC)  $(C_FLAGS) -o $@ $^ $(FT_PRINTF_PATH)/libftprintf.a $(LIBFT_PATH)/libft.a

clean:
	@echo "\033[0;33mdeleting objects... \033[0;37m"
	@rm -f $(SERVER_OBJ) $(CLIENT_OBJ)
	@make clean -C $(LIBFT_PATH)
	@make clean -C $(FT_PRINTF_PATH)
	@echo "\033[0;33mdone \033[0;37m"

fclean: clean
	@echo "\033[0;33mdeleting executable... \033[0;37m"
	@rm -f $(SERVER_NAME) $(CLIENT_NAME)
	@make fclean -C $(LIBFT_PATH)
	@make fclean -C $(FT_PRINTF_PATH)
	@echo "\033[0;33mdone \033[0;37m"

re: fclean all