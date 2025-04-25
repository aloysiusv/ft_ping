/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_pong.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:35:54 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/25 19:42:05 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

int play_ping_pong(t_parser *args, t_ping *ping, int sockfd, struct addrinfo *resolved) {
    
    struct timeval start, end;
    struct sockaddr_in recv_addr;
    socklen_t recv_len;
    char buffer[RESPONSE_SIZE];
    
    // Start clock
    gettimeofday(&start, NULL);
    if (sendto(sockfd, &ping->packet, sizeof(ping->packet), 0, resolved->ai_addr, resolved->ai_addrlen) < 0)
        return -1;
    recv_len = sizeof(recv_addr);
    memset(buffer, 0, sizeof(buffer));
    int bytes = recvfrom(sockfd, buffer, RESPONSE_SIZE, 0, (struct sockaddr*)&recv_addr, &recv_len);
    if (bytes < 0)
        return -1;
    gettimeofday(&end, NULL);
    
    // Print current infos
    double rtt = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    if (!(args->flags & OPT_QUIET))
        printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", bytes - IP_HDR_SIZE, ping->ip_dest, ping->packet.seq, ((struct ip *)buffer)->ip_ttl, rtt);
    
    // Error handling
    
    
    return 0;
}