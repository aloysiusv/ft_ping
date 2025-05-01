/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_pong.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:35:54 by lrandria          #+#    #+#             */
/*   Updated: 2025/05/01 22:34:27 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static int receive_pong(int sockfd, t_response *rsp) {
    
    int ret;
    struct sockaddr_in recv_addr;
    socklen_t recv_len = sizeof(recv_addr);
    
    memset(rsp, 0, sizeof(t_response));
    ret = recvfrom(sockfd, rsp->buffer, RESPONSE_SIZE, 0, (struct sockaddr *)&recv_addr, &recv_len);
    
    // Parse response headers
    rsp->ip_hdr = (struct iphdr *)rsp->buffer;
    rsp->ip_hdr_len = rsp->ip_hdr->ihl * 4;
    rsp->icmp_hdr = (struct icmphdr *)(rsp->buffer + rsp->ip_hdr_len);
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
    return ret; // -2 = sigint, -1 = error, 0 = timeout, 1 = ready to read
}

static int send_ping(t_parser *args, t_ping *ping, struct addrinfo *resolved) {
    
    if (sendto(ping->sockfd, &ping->packet, sizeof(ping->packet), 0, resolved->ai_addr, resolved->ai_addrlen) < 0)
        return -1;
    int fd_ready = wait_for_reply(ping->sockfd, args->timeout * 1000); // Timeout in milliseconds
    return fd_ready;
}

int play_ping_pong(t_parser *args, t_ping *ping) {
    
    int ret_send, ret_rcv;
    struct timeval start, end;
    
    // Start clock and send ICMP request
    gettimeofday(&start, NULL);
    ret_send = send_ping(args, ping, ping->resolved);
    ret_rcv = receive_pong(ping->sockfd, &ping->response);
    if (ret_send <= 0 || ret_rcv <= 0)
        return -1;
    gettimeofday(&end, NULL);

    // Check if we have the "success" header
    if (ping->response.icmp_hdr->type == ICMP_ECHOREPLY) {
	    double rtt = (end.tv_sec - start.tv_sec) * 1000.0 +
	             (double)(end.tv_usec - start.tv_usec) / 1000.0;

	    if (!(args->flags & OPT_QUIET))
		    print_current_infos(ping, &ping->response, ret_rcv, rtt);

	    // Update RTT stats
	    if (rtt < ping->rtt_min || ping->rtt_min == 0.0)
		    ping->rtt_min = rtt;
	    if (rtt > ping->rtt_max)
		    ping->rtt_max = rtt;
	    ping->rtt_sum += rtt;
	    ping->rtt_sum_sqr += rtt * rtt;
    } 
    else {
	    print_errors(ping, ret_rcv, args->flags);
	    return -1;
    }
    return 0;
}