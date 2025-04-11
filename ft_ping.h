#ifndef FT_PING_H
#define FT_PING_H

#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <stdbool.h>

#include <limits.h>

// FLAGS FOR OPTIONS
# define OPT_COUNT		0b1
# define OPT_TTL		0b10
# define OPT_VERBOSE	0b100
# define OPT_LINGER		0b1000
# define OPT_PKT_SIZE	0b10000
# define OPT_QUIET		0b100000

typedef struct {
    unsigned int flags;
    int count;
    int ttl;
    int linger_time;
    int pkt_size;
    char *destination;
} t_options;

#endif
