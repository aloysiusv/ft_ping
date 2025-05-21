/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_print.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 10:50:43 by lrandria          #+#    #+#             */
/*   Updated: 2025/05/21 12:57:00 by lrandria         ###   ########.fr       */
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
					"   -W,                        (linger) number of seconds to wait for response\n"
					"   -v,                        verbose output\n"
					"   -q,                        quiet output\n"
					"   -?, --help OR --usage      display this help list\n");
}

void print_start_infos(const  t_parser *args, const t_ping *ping) {

	if (args->flags & OPT_VERBOSE) {
		int pid = getpid() & 0xFFFF;
		fprintf(stdout, "PING %s (%s): %d data bytes, id 0x%x = %d\n", args->dest, ping->ip_dest, PAYLOAD_SIZE, pid, pid);
	}
	else
		fprintf(stdout, "PING %s (%s): %d data bytes\n", args->dest, ping->ip_dest, PAYLOAD_SIZE);
}

void print_response_infos(const t_response *rsp, const int bytes, const double rtt) {

	char ip_addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &rsp->ip_hdr->saddr, ip_addr, sizeof(ip_addr));

    fprintf(stdout, "%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
        bytes - rsp->ip_hdr_len,
        ip_addr,
        rsp->icmp_hdr->un.echo.sequence,
        rsp->ip_hdr->ttl,
        rtt
    );
}

void print_errors(t_ping *ping, const int bytes,const int flags) {

	t_response *rsp = &ping->response;
	
	if (rsp->icmp_hdr->type == ICMP_TIME_EXCEEDED)
		fprintf(stdout, "%d bytes from %s: Time to live exceeded\n", bytes - rsp->ip_hdr_len, inet_ntoa((struct in_addr){rsp->ip_hdr->saddr}));
	else if (rsp->icmp_hdr->type == ICMP_DEST_UNREACH) // Didn't bother to handle specific sub-codes :/
		fprintf(stdout, "%d bytes from %s: Destination Host Unreachable\n", bytes - rsp->ip_hdr_len, inet_ntoa((struct in_addr){rsp->ip_hdr->saddr}));
	
	if (flags & OPT_VERBOSE) {
		// Parse the second IP header (you get two when getting an error)
		struct iphdr *inner_ip = (struct iphdr *)(rsp->icmp_hdr + 1);

		// Check response size, in case it's a bad header
		if ((uint8_t *)inner_ip - (uint8_t *)rsp->buffer + sizeof(struct iphdr) <= RESPONSE_SIZE) {
			const uint8_t  *buf = (const uint8_t *)inner_ip;
			int            inner_ip_len = inner_ip->ihl * 4;
			
			// Get src and dst properly
			char src[INET_ADDRSTRLEN];
			char dst[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &inner_ip->saddr, src, sizeof(src));
			inet_ntop(AF_INET, &inner_ip->daddr, dst, sizeof(dst));

			fprintf(stdout, "IP Hdr Dump:\n");
			for (int i = 0; i < inner_ip_len; i += 2)
				fprintf(stdout, " %02x%02x ", buf[i], buf[i + 1]);
			fprintf(stdout, "\n");

			fprintf(stdout, "Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src	Dst	Data\n");
			fprintf(stdout, " %x  %x  %02x %04x %04x   %x %04x  %02x  %02d %04x %s  %s\n",
				inner_ip->version,
				inner_ip->ihl,
				inner_ip->tos,
				ntohs(inner_ip->tot_len),
				ntohs(inner_ip->id),
				ntohs(inner_ip->frag_off) >> 13,
				(ntohs(inner_ip->frag_off) & 0x1FFF),
				inner_ip->ttl,
				inner_ip->protocol,
				ntohs(inner_ip->check),
				src,
				dst
			);
			fprintf(stdout, "ICMP: type %d, code %d, size %d, id 0x%04x, seq 0x%04x\n",
				ping->packet.type,
				ping->packet.code,
				PACKET_SIZE,
				ping->packet.id,
				ping->packet.seq
			);
		}
	}
}

void print_end_infos(const t_ping *ping, char *dest) {
	
	int     total_packets = ping->packets_sent + ping->packets_lost;
	double  mean = ping->rtt_sum / total_packets;
	double  stddev = sqrt((ping->rtt_sum_sqr / total_packets) - pow(mean, 2));
	int     loss = ping->packets_lost / total_packets * 100;

	fprintf(stdout, "--- %s ft_ping statistics ---\n", dest);
	fprintf(stdout, "%d packets transmitted, %d packets received, %d%% packet loss\n", total_packets, ping->packets_sent, loss);
	if (loss != 100)
		fprintf(stdout, "round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",ping->rtt_min, mean, ping->rtt_max, stddev);
}