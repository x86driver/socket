#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

const char *NAME = "./my_sock";
#define MAX 1024

socklen_t clnt_len;
int orig_sock, new_sock;
static struct sockaddr_in clnt_adr, serv_adr;
const int PORT = 2002;

void clean_up(int , const char*);

void start_socket()
{
	if ((orig_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
	}
}

void start_bind()
{
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(PORT);
//	strcpy(serv_adr.sun_path, NAME);
//	unlink(NAME);

	if (bind(orig_sock, (struct sockaddr *)&serv_adr,
		sizeof(serv_adr)) < 0) {
		perror("bind error");
		clean_up(orig_sock, NAME);
	}
}

void start_listen()
{
	listen(orig_sock, 5);
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

	do {
		printf("Input a string: ");
		scanf("%s", clnt_buf);
		write(new_sock, clnt_buf, strlen(clnt_buf));
	} while (clnt_buf[0] != '0');

	close(new_sock);
	clean_up(orig_sock, NAME);
	return 0;
}

void clean_up(int sd, const char *the_file)
{
	close(sd);
	unlink(the_file);
}

