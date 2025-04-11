#include "ft_ping.h"

void print_options(t_options *options) {
    printf("Options:\n");
    
    if (options->flags & OPT_COUNT) {
        printf("  Count: %d\n", options->count);
    }
    if (options->flags & OPT_TTL) {
        printf("  TTL: %d\n", options->ttl);
    }
    if (options->flags & OPT_VERBOSE) {
        printf("  Verbose option is set.\n");
    }
    if (options->flags & OPT_LINGER) {
        printf("  Linger time: %d\n", options->linger_time);
    }
    if (options->flags & OPT_PKT_SIZE) {
        printf("  Packet size: %d\n", options->pkt_size);
    }
    if (options->flags & OPT_QUIET) {
        printf("  Quiet option is set.\n");
    }
}

static void parse_arg(const char *str, int *option) {

	char *endptr;
	long val;

	val = strtol(str, &endptr, 10);
	if (endptr == str) {
		fprintf(stderr, "ft_ping: invalid value (`%s' near `%s'): no digits were found\n", str, str);
		exit(EXIT_FAILURE);
	}
	if (*endptr != '\0') {
		fprintf(stderr, "ft_ping: invalid value (`%s' near `%s'): extra characters\n", str, str);
		exit(EXIT_FAILURE);
	}
	if (val > __UINT16_MAX__ || val < 0) {
		fprintf(stderr, "ft_ping: option value too big\n");
		exit(EXIT_FAILURE);
	}
	*option = val;
}

static void validate_opt(t_options *options, int flag, char *arg) {
	
	if ((options->flags & flag) == 0)
		options->flags |= flag;
	if (flag == OPT_COUNT)
		parse_arg(arg, &options->count);
	if (flag == OPT_LINGER)
		parse_arg(arg, &options->linger_time);
	if (flag == OPT_TTL)
		parse_arg(arg, &options->ttl);
	if (flag == OPT_PKT_SIZE)
		parse_arg(arg, &options->pkt_size);
}

static void get_options(int argc, char *argv[], t_options *options) {

	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] == '-') {
			if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "--usage") == 0) {
				fprintf(stdout, "will print help\n");
				exit(EXIT_SUCCESS);
			}
			else if (strcmp(argv[i], "-c") == 0) {
				if (argv[i + 1] == NULL) {
					fprintf(stderr, "ft_ping: option requires an argument -- 'c'\nTry 'ping -?', 'ping --help' or 'ping --usage' for more information\n");
					exit(EXIT_FAILURE);
				}
				validate_opt(options, OPT_COUNT, argv[++i]);
			}				
			else if (strcmp(argv[i], "-W") == 0) {
				if (argv[i + 1] == NULL) {
					fprintf(stderr, "ft_ping: option requires an argument -- 'W'\nTry 'ping -?', 'ping --help' or 'ping --usage' for more information\n");
					exit(EXIT_FAILURE);
				}
				validate_opt(options, OPT_LINGER, argv[++i]); 
			}
			else if (strncmp(argv[i], "--ttl=", 6) == 0) {
				if (strcmp(argv[i], "--ttl=") == 0) {
					fprintf(stderr, "ft_ping: option value too small:\n");
					exit(EXIT_FAILURE);
				}
				validate_opt(options, OPT_TTL, argv[i] + 6);
			}
			else if (strcmp(argv[i], "-s") == 0) {
				if (argv[i + 1] == NULL) {
					fprintf(stderr, "ft_ping: option requires an argument -- 's'\nTry 'ping -?', 'ping --help' or 'ping --usage' for more information\n");
					exit(EXIT_FAILURE);
				}
				validate_opt(options, OPT_PKT_SIZE, argv[++i]);
			}
			else if (strcmp(argv[i], "-v") == 0)
				validate_opt(options, OPT_VERBOSE, NULL);
			else if (strcmp(argv[i], "-q") == 0)
				validate_opt(options, OPT_QUIET, NULL);
			else {
				fprintf(stderr, "ft_ping: unrecognised option '%s'\nTry 'ping -?', 'ping --help' or 'ping --usage' for more information\n", argv[i]);
				exit(EXIT_FAILURE);
			}
		} else {
			printf("Destination: %s\n", argv[i]); // tmp
		}
	}
}

int main(int argc, char *argv[]) {

	if (geteuid() != 0) {
        fprintf(stderr, "ft_ping: Are you sudo...?\n");
        exit(EXIT_FAILURE);
    }
	if (argc < 2) {
        fprintf(stderr, "ft_ping: missing host operand\nTry 'ft_ping -?', 'ft_ping --help' or 'ft_ping --usage' for more information\n");
        exit(EXIT_FAILURE);
    }
	t_options options;
	memset(&options, 0, sizeof(t_options));
	get_options(argc, argv, &options);
	print_options(&options);
	return 0;
}
