#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <stdlib.h>

const char *NAME = "./my_sock";
#define MAX 1024

socklen_t clnt_len;
int orig_sock, new_sock;
static struct sockaddr_in clnt_adr, serv_adr;
const int PORT = 2010;

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

#define SIZE 1048576*8
int main(int argc, char **argv)
{
	char *clnt_buf;
        char recv_buf[MAX];
        ssize_t ret;

        clnt_buf = malloc(SIZE);
        memset(clnt_buf, 'A', SIZE);

	start_socket();
	start_bind();
	start_listen();
	start_accept();

        for (;;) {
                read(new_sock, &recv_buf, 1);
                while (recv_buf[0] == '1') {
                        ret = 0;
                        do {
                                ret += write(new_sock, clnt_buf, SIZE - ret);
                                printf("write %lu bytes\n", ret);
                        } while (ret != SIZE);
                        read(new_sock, &recv_buf, 1);
                }
                printf("Client ask me to close\n");
                close(new_sock);
                printf("Wait for another connection\n");
                start_accept();
        }

	close(new_sock);
        free(clnt_buf);
        clean_up(orig_sock, NAME);
	return 0;
}

void clean_up(int sd, const char *the_file)
{
	close(sd);
	unlink(the_file);
}

