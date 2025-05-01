/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:40:50 by lrandria          #+#    #+#             */
/*   Updated: 2025/05/01 23:35:59 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void print_help() {
   
	fprintf(stdout, "Usage: ft_ping [OPTION...] HOST ...\n"
                    "Send ICMP ECHO_REQUEST packets to network hosts.\n\n"
                    "Options:\n\n"
                    "   -c,                        (count) stop after sending X packets\n"
                    "   -i,                        (interval) wait X seconds between sending each packet\n"
                    "   --ttl=N                    specify N as time-to-live\n"
                    "   -w,                        stop after X seconds\n"
                    "   -v,                        verbose output\n"
                    "   -q,                        quiet output\n"
                    "   -?, --help OR --usage      give this help list\n");
}

void print_start_infos(const  t_parser *args, const t_ping *ping) {

	if (args->flags & OPT_VERBOSE) {
		int packet_id = getpid() & 0xFFFF;
		printf("PING %s (%s): 56 data bytes, id 0x%x = %d\n", args->dest, ping->ip_dest, packet_id, packet_id);
	}
	else
		printf("PING %s (%s): 56 data bytes\n", args->dest, ping->ip_dest);
}

void print_current_infos(const t_ping *ping, const t_response *rsp, const int bytes, const double rtt) {
    printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
        bytes - rsp->ip_hdr_len,
        ping->ip_dest,
        ping->packet.seq,
        rsp->ip_hdr->ttl,
        rtt
    );
}

void print_errors(t_ping *ping, const int bytes,const int flags) {

    t_response *rsp = &ping->response;
    
    if (rsp->icmp_hdr->type == ICMP_TIME_EXCEEDED)
        printf("%d bytes from %s: Time to live exceeded\n", bytes - rsp->ip_hdr_len, inet_ntoa((struct in_addr){rsp->ip_hdr->saddr}));
    else if (rsp->icmp_hdr->type == ICMP_DEST_UNREACH)
        printf("icmp_seq=%d Host Unreachable\n", ping->packet.seq);
    else if (rsp->icmp_hdr->type == ICMP_REDIRECT)
        printf("icmp_seq=%d Redirect Host\n", ping->packet.seq);
    else if (rsp->icmp_hdr->type == ICMP_PARAMETERPROB)
        printf("icmp_seq=%d Parameter problem\n", ping->packet.seq);
    
    if (flags & OPT_VERBOSE) {
        const uint8_t *buf = (const uint8_t *)rsp->buffer;
        printf("IP Hdr Dump:\n");
        for (int i = 0; i < 20; i += 2)
            printf("%02x%02x ", buf[i], buf[i + 1]);
        printf("\n");
        printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src    Dst\n");
        printf("%x  %x  %02x %04x %04x   %x %04x %02x  %3d %04x %s %s\n",
            rsp->ip_hdr->version,
            rsp->ip_hdr->ihl,
            rsp->ip_hdr->tos,
            ntohs(rsp->ip_hdr->tot_len),
            ntohs(rsp->ip_hdr->id),
            ntohs(rsp->ip_hdr->frag_off) >> 13,
            (ntohs(rsp->ip_hdr->frag_off) & 0x1FFF),
            rsp->ip_hdr->ttl,
            rsp->ip_hdr->protocol,
            ntohs(rsp->ip_hdr->check),
            inet_ntoa((struct in_addr){rsp->ip_hdr->saddr}),
            inet_ntoa((struct in_addr){rsp->ip_hdr->daddr})
        );
        printf("ICMP: type %d, code %d, size %d, id 0x%04x, seq 0x%04x\n",
            ping->packet.type,
            ping->packet.code,
            PACKET_SIZE,
            ping->packet.id,
            ping->packet.seq
        );
    }
}
    
void print_end_infos(const t_ping *ping) {
    // double mean = ping->rtt_sum / ping->rtt_count;
    // double stddev = sqrt((ping->rtt_sqr_sum / ping->rtt_count) - pow(mean, 2));
    // int loss = 100 - (((ping->seq + 1) - ping->errors) /(ping->seq + 1)) * 100;
    // if (loss == 100)
    //     mean = stddev = 0;
    (void)ping;
    printf("\n--- ft_ping statistics ---\n");
    // printf("%d packets transmitted, %d packets received, %d%% packet loss\n", ping->rtt_count + ping->errors, ping->rtt_count, loss);
    // printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", ping->rtt_min, mean, ping->rtt_max, stddev);
}