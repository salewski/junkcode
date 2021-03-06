#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <ctype.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <fnmatch.h>
#include <strings.h>
#include <dlfcn.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>

#include "cgi.h"
#include "template.h"

typedef unsigned BOOL;
#define True 1
#define False 0

#define TSERVER_PORT 80

#define MMAP_FAILED ((void *)-1)

/* prototypes */
void tcp_listener(int port, void (*fn)(void));

void *map_file(const char *fname, size_t *size);
void unmap_file(void *p, size_t size);
void *x_malloc(size_t size);
void trim_tail(char *s, char *trim_chars);

