/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_ping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:58:35 by lrandria          #+#    #+#             */
/*   Updated: 2025/05/01 19:28:10 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

extern volatile sig_atomic_t g_stop;

static t_packet init_packet(int i) {
	
	t_packet packet;
	
	memset(&packet, 0, sizeof(packet));
	packet.type = ICMP_ECHO;
	packet.code = 0;
	packet.id = getpid() & 0xFFFF;
	packet.seq = i;
	packet.checksum = checksum(&packet, sizeof(packet));
	return packet;
}

static char *get_ip(struct addrinfo *resolved) {
	
	static char ip_addr[INET_ADDRSTRLEN];
	struct sockaddr_in *tmp = (struct sockaddr_in *)resolved->ai_addr;
	
	inet_ntop(AF_INET, &tmp->sin_addr, ip_addr, sizeof(ip_addr));
	return ip_addr;
}

static struct addrinfo *resolve_addr(char *dest) {
	
	struct addrinfo	hints;
	struct addrinfo	*resolved;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

	if (getaddrinfo(dest, NULL, &hints, &resolved) != 0)
		oops_crash(E_BAD_DEST, NULL);
	return resolved;
}

static int init_socket(t_parser *args) {
	
	int ret = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	
	if (ret < 0)
		oops_crash(E_INTERNAL_ERROR, NULL);
	setsockopt(ret, IPPROTO_IP, IP_TTL, &args->ttl, sizeof(args->ttl));
	return ret;
}

void start_ping(t_parser *args, t_ping *ping) {
	
	// Preparing socket and dest address
	ping->sockfd = init_socket(args);
	ping->resolved = resolve_addr(args->dest);
	ping->ip_dest = get_ip(ping->resolved);
	print_start_infos(args, ping);
	
	// Main loop
	for (int i = 0; i != args->packet_count; i++) {
		ping->packet = init_packet(i);
		if (play_ping_pong(args, ping) == -1)
			ping->packets_lost++;
		sleep(args->interval);
		if (g_stop == 1) {
			break;
		}
	}
	print_end_infos(ping);
	freeaddrinfo(ping->resolved);
}
