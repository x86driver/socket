#ifndef _SERVER_H
#define _SERVER_H

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define dbg printf

class SocketServer {
public:
	SocketServer() : orig_sock(0), new_sock(0), fd(0) {}
	ssize_t read(void *buf, size_t count) {
		return ::read(new_sock, buf, count);
	}
	ssize_t write(const void *buf, size_t count) {
		return ::write(new_sock, buf, count);
	}
protected:
	~SocketServer();
	void start();
private:
	void clean_up(int sd, const char *the_file);
	int orig_sock, new_sock;
	int fd;
	socklen_t clnt_len;
	struct sockaddr_un clnt_adr, serv_adr;
	pthread_t tid;
};

class UartTransport {
public:
	void start() {
		printf("open uart, set bit rate\n");
	}
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
	void read_from_client(void *buf, size_t count) {
		ServerPolicy::read(buf, count);
	}
	void send_to_client(const void *buf, size_t count) {
		ServerPolicy::write(buf, count);
	}
private:
};

#endif

