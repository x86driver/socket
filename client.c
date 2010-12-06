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

int orig_sock;
//int main()
int socket_init(void)
{
	struct sockaddr_in serv_adr;
	struct hostent *host;

	host = gethostbyname("localhost");

	if ((orig_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
		return 1;
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	memcpy(&serv_adr.sin_addr, host->h_addr, host->h_length);
	serv_adr.sin_port = htons(PORT);

	if (connect(orig_sock, (struct sockaddr*)&serv_adr,
		sizeof(serv_adr)) < 0) {
		perror("connect error");
		return 2;
	}

	return 0;
}

int main()
{
	char buf[MAX];
        int ret;

	socket_init();

        buf[0] = '1';
        printf("Sending a request\n");
        write(orig_sock, &buf[0], 1);
	printf("Waiting for server...\n");
        read(orig_sock, &buf[0], 1);
        printf("%c\n", buf[0]);

        buf[0] = '1';
        printf("Sending a request\n");
        write(orig_sock, &buf[0], 1);
        printf("Waiting for server...\n");
        read(orig_sock, &buf[0], 1);
        printf("%c\n", buf[0]);

        buf[0] = '0';
        printf("Sending end request, bye~\n");
        write(orig_sock, &buf[0], 1);

#if 0
	do {
		ret = read(orig_sock, &buf[0], 1);
		if (ret != 0)
			printf("%c", buf[0]);
			fflush(NULL);
	} while (ret > 0);

	strcpy(buf, MSG);
	buf[strlen(MSG)] = '\0';
	write(orig_sock, buf, strlen(MSG)+1);
#endif

	printf("\n");
	close(orig_sock);
	return 0;
}

