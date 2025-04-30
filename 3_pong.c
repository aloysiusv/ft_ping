/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_pong.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:35:54 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/30 14:48:14 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static int receive_pong(t_ping *ping, char *buffer) {
    
    int ret;
    struct sockaddr_in recv_addr;
    socklen_t recv_len = sizeof(recv_addr);
    
    ret = recvfrom(ping->sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&recv_addr, &recv_len);
    
    return ret;
}

static int wait_for_reply(int sockfd, int timeout_ms) {
    
    fd_set readfds;
    struct timeval timeout;

    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    int ret = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
    if (ret == -1) {
        if (errno == EINTR)
            return -2; // Interrupted by signal
        perror("select");
        return -1;
    }
    return ret; // 0 = timeout, 1 = ready to read
}

static int send_ping(t_parser *args, t_ping *ping, struct addrinfo *resolved) {
    
    if (sendto(ping->sockfd, &ping->packet, sizeof(ping->packet), 0, resolved->ai_addr, resolved->ai_addrlen) < 0)
        return -1;
    int fd_ready = wait_for_reply(ping->sockfd, args->timeout * 1000); // Timeout in milliseconds
    if (fd_ready == 0) { // Timeout
        fprintf(stderr, "Request timeout for icmp_seq %d\n", ping->packet.seq);
        return 0;
    }
    if (fd_ready < 0)
        return -1;
}

int play_ping_pong(t_parser *args, t_ping *ping) {
    
    int ret_send, ret_rcv;
    struct timeval start, end;
    char buffer[RESPONSE_SIZE];
    
    // Start clock and send ICMP request
    gettimeofday(&start, NULL);
    ret_send = send_ping(args, ping, ping->resolved);
    ret_rcv = receive_pong(ping, buffer);
    if (ret_send < 0 || ret_rcv < 0)
        return -1;
    gettimeofday(&end, NULL);

    // Parse IP and ICMP headers
    struct ip *ip_hdr = (struct ip *)buffer;
    int ip_hdr_len = ip_hdr->ip_hl << 2;
    struct icmphdr *icmp_hdr = (struct icmphdr *)(buffer + 20);

    // Check ICMP reply
    if (icmp_hdr->type == ICMP_ECHOREPLY) {
        double rtt = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
        if (!(args->flags & OPT_QUIET)) {
            printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                ret_rcv - ip_hdr_len,
                ping->ip_dest,
                ping->packet.seq,
                ip_hdr->ip_ttl,
                rtt
            );
        }
    } else {
        print_errors(args->flags, ping->packet.seq, buffer, ret_rcv);
    }
    return 0;
}