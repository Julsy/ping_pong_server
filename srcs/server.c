/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iiliuk <iiliuk@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/21 10:44:47 by iiliuk            #+#    #+#             */
/*   Updated: 2017/02/21 10:55:24 by iiliuk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "libft/libft.h"

int				ft_printf(char *fmt, ...);

static void     error(const char *msg)
{
    perror(msg);
    exit(1);
}

static void     receive_message(int newsockfd)
{
    int     done;
    char    buffer[256];
    int     n;

    done = 0;
    while (!done)
    {
        ft_bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        if (n <= 0)
        {
            perror("ERROR reading from socket");
            done = 1;
        }
        if (ft_strcmp(buffer, "ping") == 0)
        {
            ft_printf("Command received: %s\n",buffer);
            ft_strcpy(buffer, "pong pong");
        }
        else
            ft_printf("Wrong command received: %s\n 'ping' expected\n", buffer);
        n = write(newsockfd, buffer, ft_strlen(buffer));
        if (n < 0) error("ERROR writing to socket");
    }
}

static int      bind_listen(int sockfd, struct sockaddr_in serv_addr, struct sockaddr_in cli_addr)
{
    int         newsockfd;
    socklen_t   clilen;

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");
    return (newsockfd);
}

static int        prepare_socket(char *argv[])
{
    int sockfd;
    int newsockfd;
    int portno;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    ft_bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = ft_atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    newsockfd = bind_listen(sockfd, serv_addr, cli_addr);
    close(sockfd);
    return (newsockfd);
}

int         main(int argc, char *argv[])
{
    int newsockfd;

    if (argc < 2)
    {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    while (1)
    {
        newsockfd = prepare_socket(argv);
        receive_message(newsockfd);
        close(newsockfd);
    }
    return (0); 
}
