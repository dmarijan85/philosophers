# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dmarijan <dmarijan@student.42barcel>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/03 11:33:35 by dmarijan          #+#    #+#              #
#    Updated: 2024/07/25 16:17:04 by dmarijan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRC = dinner.c gsetters.c initialiser.c main.c parser.c philoutils.c \
		security.c synchro_utils.c write.c monitor.c 

OBJ = $(SRC:.c=.o)

COMPILE = cc -Wall -Werror -Wextra -g #-fsanitize=address

REMOVE = rm -f

all: $(NAME)

$(NAME): Makefile $(OBJ) philo.h
	@$(COMPILE) $(SRC) -o $(NAME)

clean:
	$(REMOVE) $(OBJ)

fclean: clean
	$(REMOVE) $(NAME)

re: fclean all

.PHONY = all clean fclean re
