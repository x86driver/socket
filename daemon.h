#ifndef _SERVER_H
#define _SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

struct NativeReadWrite {
	void set(int fd) {
		_fd = fd;
	}
	ssize_t read(void *buf, size_t count) {
//		return ::read(fd_, buf, count);
		printf("fd: %d\n", _fd);
		return 0;
	}
	ssize_t write(const void *buf, size_t count) {
		return ::write(_fd, buf, count);
	}
private:
	int _fd;
};

template <typename TransmissionPolicy>
class SocketServer : public TransmissionPolicy {
public:
	int start_server() {
		TransmissionPolicy::set(10);
		orig_sock = 101;
		return 0;
	}
	void show() {printf("%d\n", orig_sock);}
protected:
//	~SocketServer() {printf("WOWO\n"); }
private:
	socklen_t clnt_len;
	int orig_sock, new_sock;
	struct sockaddr_un clnt_adr, serv_adr;
};

template
<
	template <class nouse>
	typename ServerPolicy
>
class Daemon : public ServerPolicy<NativeReadWrite> {
public:
	void start() {
		ServerPolicy::start_server();
	}
	~Daemon() {printf("daemon cow\n");}
private:
};
#endif

