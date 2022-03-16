# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: apommier <apommier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/13 16:27:49 by apommier          #+#    #+#              #
#    Updated: 2022/03/15 17:49:12 by apommier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo
SRCS	= srcs/main.c\
			srcs/init.c\
			srcs/exit.c\
			srcs/philo.c\
			srcs/utils.c
OBJS	= ${SRCS:.c=.o}
CC		= gcc
CFLAGS	= -pthread -Wall -Wextra -Werror
RM		= rm -rf

${NAME}: ${OBJS} 
		@${CC} ${CFLAGS} ${OBJS} -o ${NAME}
	
all:	${NAME}

clean:
		@${RM} ${OBJS}

fclean:	clean
		@${RM} ${NAME}

		
re: 	fclean all

.PHONY: all clean fclean re bonus 