/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:40:35 by lrandria          #+#    #+#             */
/*   Updated: 2025/05/03 11:00:57 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

volatile sig_atomic_t g_stop = 0;

static void init_default(t_parser *args) {
    // DEFAULT VALUES
    args->packet_count = -1; // Necessary for infinite loop
    args->ttl = 64;
    args->linger = 1;
    args->interval = 1;
}

void oops_crash(const char* msg, const char* try_help) {
    
    fprintf(stderr, "%s", msg);
    if (try_help)
        fprintf(stderr, "%s", try_help);
    exit(EXIT_FAILURE);
}

static void sig_handler(int sig) {
    (void)sig;
    g_stop = 1; // For a cleaner exit
}

int main(int ac, char *av[]) {

    t_parser    args;
    t_ping      ping;
    
    signal(SIGINT, sig_handler);
    
    if (geteuid() != 0)
        oops_crash(E_NOT_SUDO, NULL);
    if (ac < 2)
        oops_crash(E_MISSING_DEST, E_TRY_HELP);
    
    memset(&args, 0, sizeof(t_parser));
    memset(&ping, 0, sizeof(t_ping));
    init_default(&args);
    parse_args(ac, av, &args);
    start_ping(&args, &ping);
    return 0;
}
