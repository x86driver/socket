#ifndef _SERVER_H
#define _SERVER_H

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "uart_io.h"

#define dbg printf

class SocketServer {
public:
	SocketServer() : orig_sock(0), new_sock(0), fd(0) {}
protected:
	ssize_t read(void *buf, size_t count) {
		return ::read(new_sock, buf, count);
	}
	ssize_t write(const void *buf, size_t count) {
		return ::write(new_sock, buf, count);
	}
	void start();
	~SocketServer();
private:
	void clean_up(int sd, const char *the_file);
	int orig_sock, new_sock;
	int fd;
	socklen_t clnt_len;
	struct sockaddr_un clnt_adr, serv_adr;
	pthread_t tid;
};

template
<
	typename ServerPolicy = SocketServer,
	typename TransportPolicy = UartTransport
>
class Daemon : public ServerPolicy, public TransportPolicy {
public:
	void start_server() {
		ServerPolicy::start();
	}
	void start_transport() {
		TransportPolicy::start();
	}
	void stop_transport() {
		TransportPolicy::stop();
	}
	ssize_t server_read(void *buf, size_t count) {
		return ServerPolicy::read(buf, count);
	}
	ssize_t server_write(const void *buf, size_t count) {
		return ServerPolicy::write(buf, count);
	}
	ssize_t transport_read(void *buf, size_t count) {
		return TransportPolicy::read(buf, count);
	}
	ssize_t transport_write(void *buf, size_t count) {
		return TransportPolicy::write(buf, count);
	}
private:
};

#endif

