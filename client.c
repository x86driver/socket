#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

const char *NAME = "./my_sock";
#define MSG "Hello world!"
#define MAX 1024

int main()
{
	int orig_sock;
	static struct sockaddr_un serv_adr;
	static char buf[MAX];

	if ((orig_sock = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
		return 1;
	}

	serv_adr.sun_family = AF_UNIX;
	strcpy(serv_adr.sun_path, NAME);

	if (connect(orig_sock, (struct sockaddr *)&serv_adr,
		sizeof(serv_adr.sun_family)+strlen(serv_adr.sun_path)) < 0) {
		perror("connect error");
		return 2;
	}

//	strcpy(buf, MSG);
//	write(orig_sock, buf, strlen(MSG));
	printf("Waiting for server...\n");
	int ret = read(orig_sock, buf, MAX);
	printf("read ret: %d\n", ret);
	printf("String: %s\n", buf);

	close(orig_sock);
	return 0;
}

