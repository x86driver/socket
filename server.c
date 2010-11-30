#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

const char *NAME = "./my_sock";
#define MAX 1024

socklen_t clnt_len;
int orig_sock, new_sock;
static struct sockaddr_un clnt_adr, serv_adr;

void clean_up(int , const char*);

void start_socket()
{
	if ((orig_sock = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
	}
}

void start_bind()
{
	serv_adr.sun_family = AF_UNIX;
	strcpy(serv_adr.sun_path, NAME);
	unlink(NAME);

	if (bind(orig_sock, (struct sockaddr *)&serv_adr,
		sizeof(serv_adr.sun_family)+strlen(serv_adr.sun_path)) < 0) {
		perror("bind error");
		clean_up(orig_sock, NAME);
	}
}

void start_listen()
{
	listen(orig_sock, 1);
}

void start_accept()
{
	clnt_len = sizeof(clnt_adr);
	if ((new_sock = accept(orig_sock, (struct sockaddr *)&clnt_adr, &clnt_len)) < 0) {
		perror("accept error");
		clean_up(orig_sock, NAME);
	}
}

int main(int argc, char **argv)
{
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
	return 0;
}

void clean_up(int sd, const char *the_file)
{
	close(sd);
	unlink(the_file);
}

