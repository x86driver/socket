#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

const char *NAME = "./my_sock";
#define MAX 1024

void clean_up(int , const char*);

int main()
{
	socklen_t clnt_len;
	int orig_sock, new_sock;
	static struct sockaddr_un clnt_adr, serv_adr;
	static char clnt_buf[MAX], pipe_buf[MAX];
	FILE *fin;
	int i;

	if ((orig_sock = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
		return 1;
	}

	serv_adr.sun_family = AF_UNIX;
	strcpy(serv_adr.sun_path, NAME);
	unlink(NAME);

	if (bind(orig_sock, (struct sockaddr *)&serv_adr,
		sizeof(serv_adr.sun_family)+strlen(serv_adr.sun_path)) < 0) {
		perror("bind error");
		clean_up(orig_sock, NAME);
		return 2;
	}

	listen(orig_sock, 1);
	clnt_len = sizeof(clnt_adr);
	if ((new_sock = accept(orig_sock, (struct sockaddr *)&clnt_adr, &clnt_len)) < 0) {
		perror("accept error");
		clean_up(orig_sock, NAME);
		return 3;
	}

	memset(clnt_buf, 0, MAX);
	read(new_sock, clnt_buf, sizeof(clnt_buf));
	printf("%s\n", clnt_buf);

	close(new_sock);
	clean_up(orig_sock, NAME);
	return 0;
}

void clean_up(int sd, const char *the_file)
{
	close(sd);
	unlink(the_file);
}

