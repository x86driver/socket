#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>

const char *NAME = "./my_sock";
#define MSG "Hello world!"
#define MAX 1024
const int PORT = 2002;

int main()
{
	int orig_sock;
	static struct sockaddr_in serv_adr;
	static char buf[MAX];
	struct hostent *host;

	host = gethostbyname("localhost");

	if ((orig_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
		return 1;
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
//	strcpy(serv_adr.sun_path, NAME);
	memcpy(&serv_adr.sin_addr, host->h_addr, host->h_length);
	serv_adr.sin_port = htons(PORT);

//	if (connect(orig_sock, (struct sockaddr *)&serv_adr,
//		sizeof(serv_adr.sun_family)+strlen(serv_adr.sun_path)) < 0) {
	if (connect(orig_sock, (struct sockaddr*)&serv_adr,
		sizeof(serv_adr)) < 0) {
		perror("connect error");
		return 2;
	}

	printf("Waiting for server...\n");
	int ret;
	do {
		ret = read(orig_sock, &buf[0], 1);
//		printf("read ret: %d\n", ret);
//		printf("String: %s\n", buf);
		if (ret != 0)
			printf("%c", buf[0]);
			fflush(NULL);
	} while (ret > 0);

	strcpy(buf, MSG);
	buf[strlen(MSG)] = '\0';
	write(orig_sock, buf, strlen(MSG)+1);

	printf("\n");
	close(orig_sock);
	return 0;
}

