#ifndef _SERVER_H
#define _SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

class Daemon {
public:
	Daemon(const char *name);
	virtual int start_server();
	virtual int wait_con();
	virtual int send_data(void *data, size_t len);
	~Daemon();
private:
	socklen_t clnt_len;
	int orig_sock, new_sock;
	struct sockaddr_un clnt_adr, serv_adr;
};
#endif

