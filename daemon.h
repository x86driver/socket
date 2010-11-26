#ifndef _SERVER_H
#define _SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

struct NativeReadWrite {
	void setfd(int fd) {
		fd_ = fd;
	}
	ssize_t read(void *buf, size_t count) {
		return ::read(fd_, buf, count);
	}
	ssize_t write(const void *buf, size_t count) {
		return ::write(fd_, buf, count);
	}
private:
	int fd_;
};

template <typename TransmissionPolicy>
class SocketServer : public TransmissionPolicy {
public:
	int start_server() {
		TransmissionPolicy::setfd(10);
		return 0;
	}
	void send();
	void receive();
protected:
	~SocketServer() {printf("WOWO\n"); }
private:
	socklen_t clnt_len;
	int orig_sock, new_sock;
	struct sockaddr_un clnt_adr, serv_adr;
};

template
<
	typename ServerPolicy
>
class Daemon : public ServerPolicy {
public:
	void start() {
		ServerPolicy::start_server();
	}
	~Daemon() {printf("daemon cow\n");}
};
#endif

