/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:40:50 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/30 14:40:13 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

// void print_options(t_parser *options) { //debug
	
// 	printf("Options:\n");
// 	if (options->flags & OPT_COUNT) {
// 		printf("Count: %d\n", options->packet_count);
// 	}
// 	if (options->flags & OPT_TTL) {
// 		printf("TTL: %d\n", options->ttl);
// 	}
// 	if (options->flags & OPT_VERBOSE) {
// 		printf("Verbose option is set.\n");
// 	}
// 	if (options->flags & OPT_TIMEOUT) {
// 		printf("Timeout: %d\n", options->timeout);
// 	}
// 	if (options->flags & OPT_INTERVAL) {
// 		printf("Interval: %d\n", options->interval);
// 	}
// 	if (options->flags & OPT_QUIET) {
// 		printf("Quiet option is set.\n");
// 	}
// 	printf("Destinations:\n");
// 	for (int i = 0; i < options->dest_count; ++i)
// 		printf("=> %s\n", options->dest);
// }

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

void print_start_infos( t_parser *args, t_ping *ping) {

	if (args->flags & OPT_VERBOSE) {
		int packet_id = getpid() & 0xFFFF;
		printf("PING %s (%s): 56 data bytes, id 0x%x = %d\n", args->dest, ping->ip_dest, packet_id, packet_id);
	}
	else
		printf("PING %s (%s): 56 data bytes\n", args->dest, ping->ip_dest);
}

void print_errors(int flags, int seq, char *buffer, ssize_t bytes) {
    struct ip *ip_hdr = (struct ip *) buffer;
    int ip_header_len = ip_hdr->ip_hl * 4;
    struct icmphdr *icmp_hdr = (struct icmphdr *)(buffer + ip_header_len);
    
    if (icmp_hdr->type == ICMP_TIME_EXCEEDED)
        printf("%ld bytes from %s: Time to live exceeded\n", bytes, inet_ntoa(ip_hdr->ip_src));
    else if (icmp_hdr->type == ICMP_DEST_UNREACH)
        printf("icmp_seq=%d Host Unreachable\n", seq);
    else if (icmp_hdr->type == ICMP_REDIRECT)
        printf("icmp_seq=%d Redirect Host\n", seq);
    else if (icmp_hdr->type == ICMP_PARAMETERPROB)
        printf("icmp_seq=%d Parameter problem\n", seq);
    
    if (flags & OPT_VERBOSE) {
        printf("IP Hdr Dump:");
        unsigned char *ptr = (unsigned char *)ip_hdr;
        for (int i = 0; i < 20; i++) {
            if (i % 6 == 0)
                printf(" ");
            printf("%02x", ptr[i]);
        }
        printf("\n");
        printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src    Dst\n");
        printf("%x  %x  %02x %04x %04x   %x %04x %02x  %3d %04x %s %s\n",
            ip_hdr->ip_v,
            ip_hdr->ip_hl,
            ip_hdr->ip_tos,
            ntohs(ip_hdr->ip_len),
            ntohs(ip_hdr->ip_id),
            (ntohs(ip_hdr->ip_off) >> 13) & 0x7,
            ntohs(ip_hdr->ip_off) & 0x1FFF,
            ip_hdr->ip_ttl,
            ip_hdr->ip_p,
            ntohs(ip_hdr->ip_sum),
            inet_ntoa(ip_hdr->ip_src),
            inet_ntoa(ip_hdr->ip_dst)
        );
        printf("ICMP: type %d, code %d, size %ld, id 0x%04x, seq 0x%04x\n",
            icmp_hdr->type,
            icmp_hdr->code,
            bytes - ip_header_len,
            ntohs(icmp_hdr->un.echo.id),
            ntohs(icmp_hdr->un.echo.sequence)
        );
    }
}
    
    
void print_end_infos() {
	
}