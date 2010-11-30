#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "daemon.h"

const char *NAME = "./my_sock";
#define MAX 1024

void SocketServer::start()
{
	if ((orig_sock = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
	}

	serv_adr.sun_family = AF_UNIX;
	strcpy(serv_adr.sun_path, NAME);
	unlink(NAME);

	if (bind(orig_sock, (struct sockaddr *)&serv_adr,
		sizeof(serv_adr.sun_family)+strlen(serv_adr.sun_path)) < 0) {
		perror("bind error");
		clean_up(orig_sock, NAME);
	}

	listen(orig_sock, 1);

	clnt_len = sizeof(clnt_adr);
	if ((new_sock = accept(orig_sock, (struct sockaddr *)&clnt_adr, &clnt_len)) < 0) {
		perror("accept error");
		clean_up(orig_sock, NAME);
	}
}

SocketServer::~SocketServer()
{
	close(new_sock);
	clean_up(orig_sock, NAME);
}

void SocketServer::clean_up(int sd, const char *the_file)
{
        close(sd);
        unlink(the_file);
}

// =====================
Daemon<> gps_daemon;

void *library_thread(void *data)
{
	const char *txt = "Fuck you!!";
	char buf[64];
	gps_daemon.start_server();
	gps_daemon.send_to_client(txt, strlen(txt));
	gps_daemon.read_from_client(buf, sizeof(buf));
	printf("Buffer: %s\n", buf);
	return NULL;
}

void *trans_thread(void *data)
{
	gps_daemon.start_transport();
	return NULL;
}

int main(int argc, char **argv)
{
/*
	static char clnt_buf[MAX];

	start_socket();
	start_bind();
	start_listen();
	start_accept();

	printf("Input a string: ");
	scanf("%s", clnt_buf);
	write(new_sock, clnt_buf, strlen(clnt_buf));

	close(new_sock);
	clean_up(orig_sock, NAME);
*/
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, (void*(*)(void*))library_thread, NULL);
	pthread_create(&tid2, NULL, trans_thread, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	return 0;
}

