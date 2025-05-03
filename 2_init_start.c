/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_init_start.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 11:06:43 by lrandria          #+#    #+#             */
/*   Updated: 2025/05/03 12:34:20 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

extern volatile sig_atomic_t g_stop;

uint16_t checksum(void *ptr, int len)
{
    uint16_t* data = ptr;
    uint32_t  sum = 0;
    for (; len > 1; len -= 2)
        sum += *data++;
    if (len == 1)
        sum += *(uint8_t*)data;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}

static t_packet init_packet(int i) {
    
    t_packet packet;
    
    memset(&packet, 0, sizeof(packet));
    packet.type = ICMP_ECHO;
    packet.code = 0;
    packet.id = getpid() & 0xFFFF; // Making sure it fits in 16-bits range
    packet.seq = i;
    packet.checksum = checksum(&packet, sizeof(packet));
    return packet;
}

static char *get_ip(struct addrinfo *resolved) {
    
    static char         ip_addr[INET_ADDRSTRLEN];
    struct sockaddr_in  *tmp = (struct sockaddr_in *)resolved->ai_addr;
    
    inet_ntop(AF_INET, &tmp->sin_addr, ip_addr, sizeof(ip_addr));
    return ip_addr;
}

static struct addrinfo *resolve_addr(char *dest) {
    
    struct addrinfo hints;
    struct addrinfo *resolved;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    if (getaddrinfo(dest, NULL, &hints, &resolved) != 0)
        oops_crash(E_BAD_DEST, NULL);
    return resolved;
}

static int init_socket(t_parser *args) {
    
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    
    if (sockfd < 0)
        oops_crash(E_INTERNAL_ERROR, NULL);
    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &args->ttl, sizeof(args->ttl)) < 0) {
        close(sockfd);
        oops_crash(E_INTERNAL_ERROR, NULL);
    }
    return sockfd;
}

void start_ping(t_parser *args, t_ping *ping) {
    
    // Preparing socket and dest address
    ping->resolved = resolve_addr(args->dest);
    ping->ip_dest = get_ip(ping->resolved);
    ping->sockfd = init_socket(args);
    print_start_infos(args, ping);
    
    // Main loop
    for (int i = 0; i != args->packet_count; i++) {
        ping->packet = init_packet(i);
        if (play_ping_pong(args, ping) == -1)
            ping->packets_lost++;
        else
            ping->packets_sent++;
        sleep(args->interval);
        if (g_stop == 1)
            break;
    }
    print_end_infos(ping, args->dest);
    freeaddrinfo(ping->resolved);
    close(ping->sockfd);
}
