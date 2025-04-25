/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_parser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:52:57 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/25 14:40:12 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static int check_value(const char *str) {

	char *endptr;
	long val;

	val = strtol(str, &endptr, 10);
	if (endptr == str || *endptr != '\0' || val > __UINT16_MAX__ || val < 0)
        oops_crash(E_BAD_VALUE, NULL);
	return (uint16_t)val;
}

static void validate_opt(t_parser *options, int flag, char *arg) {
	
	uint16_t val;

	options->flags |= flag;
	if (arg) {
		val = check_value(arg);
		if (flag == OPT_COUNT)
			options->packet_count = val;
		else if (flag == OPT_TIMEOUT)
			options->timeout = val;
		else if (flag == OPT_TTL)
			options->ttl = val;
		else if (flag == OPT_INTERVAL)
			options->interval = val;
	}
}

void parse_args(int argc, char *argv[], t_parser *args) {

	int found_dest = 0;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "--usage") == 0) {
				print_help();
				exit(EXIT_SUCCESS);
			}
			else if (strcmp(argv[i], "-c") == 0) {
				if (argv[i + 1] == NULL)
					oops_crash(E_MISSING_ARG, E_TRY_HELP);
				validate_opt(args, OPT_COUNT, argv[++i]);
			}				
			else if (strcmp(argv[i], "-w") == 0) {
				if (argv[i + 1] == NULL)
					oops_crash(E_MISSING_ARG, E_TRY_HELP);
				validate_opt(args, OPT_TIMEOUT, argv[++i]); 
			}
			else if (strncmp(argv[i], "--ttl=", 6) == 0) {
				if (strcmp(argv[i], "--ttl=") == 0)
					oops_crash(E_TOO_SMALL, NULL);
				validate_opt(args, OPT_TTL, argv[i] + 6);
			}
			else if (strcmp(argv[i], "-i") == 0) {
				if (argv[i + 1] == NULL)
					oops_crash(E_MISSING_ARG, E_TRY_HELP);
				validate_opt(args, OPT_INTERVAL, argv[++i]);
			}
			else if (strcmp(argv[i], "-v") == 0)
				validate_opt(args, OPT_VERBOSE, NULL);
			else if (strcmp(argv[i], "-q") == 0)
				validate_opt(args, OPT_QUIET, NULL);
			else
				oops_crash(E_WTF_OPT, NULL);
			}
		else {
			if (found_dest)
				oops_crash(E_MAX_DEST, E_TRY_HELP);
			found_dest = 1;
        	args->dest = argv[i];
		}
	}
}