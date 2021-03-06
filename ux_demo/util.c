#include "ux_demo.h"


/*
  connect to a unix domain socket
*/
int ux_socket_connect(const char *name)
{
	int fd;
        struct sockaddr_un addr;

        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, name, sizeof(addr.sun_path));

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1) {
		return -1;
	}
	
	if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		close(fd);
		return -1;
	}

	return fd;
}


/*
  create a unix domain socket and start listening on it
  return a file descriptor open on the socket 
*/
int ux_socket_listen(const char *name)
{
	int fd;
        struct sockaddr_un addr;

	/* get rid of any old socket */
	unlink(name);

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1) return -1;

        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, name, sizeof(addr.sun_path));

        if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		close(fd);
		return -1;
	}	

        if (listen(fd, 10) == -1) {
		close(fd);
		return -1;
	}

	return fd;
}


void *x_malloc(size_t size)
{
	void *ret;
	ret = malloc(size);
	if (!ret) {
		fprintf(stderr,"Out of memory for size %d\n", (int)size);
		exit(1);
	}
	return ret;
}

void *x_realloc(void *ptr, size_t size)
{
	void *ret;
	ret = realloc(ptr, size);
	if (!ret) {
		fprintf(stderr,"Out of memory for size %d\n", (int)size);
		exit(1);
	}
	return ret;
}

/*
  keep writing until its all sent
*/
int write_all(int fd, const void *buf, size_t len)
{
	size_t total = 0;
	while (len) {
		int n = write(fd, buf, len);
		if (n <= 0) return total;
		buf = n + (char *)buf;
		len -= n;
		total += n;
	}
	return total;
}

/*
  keep reading until its all read
*/
int read_all(int fd, void *buf, size_t len)
{
	size_t total = 0;
	while (len) {
		int n = read(fd, buf, len);
		if (n <= 0) return total;
		buf = n + (char *)buf;
		len -= n;
		total += n;
	}
	return total;
}

/*
  send a packet in length prefix format
*/
int send_packet(int fd, const char *buf, size_t len)
{
	if (write_all(fd, &len, sizeof(len)) != sizeof(len)) return -1;
	if (write_all(fd, buf, len) != len) return -1;	
	return 0;
}

/*
  receive a packet in length prefix format
*/
int recv_packet(int fd, char **buf, size_t *len)
{
	if (read_all(fd, len, sizeof(*len)) != sizeof(*len)) return -1;
	(*buf) = x_malloc(*len);
	if (read_all(fd, *buf, *len) != *len) {
		free(*buf);
		return -1;
	}
	return 0;
}
