#include "main.h"
#include "irc.h"

int irc_send(int socketfd, char *out) {
	printf(">> %s", out);
	return send(socketfd, out, strlen(out), 0);
}

int irc_connect(char *server, int port, int *socketfd) {
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);

	struct hostent *serv_host = gethostbyname(server);
	if (serv_host == NULL) {
		return 0;
	}

	bcopy((char *)serv_host->h_addr,
		(char *)&servaddr.sin_addr.s_addr,
		serv_host->h_length);

	if((*socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		return 0;
	}

	if(connect(*socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		return 0;
	}

	return 1;
}

int irc_read(int socketfd, char *in) {
	int n;
	n = read(socketfd, in, SOCKBUFIN);
	return n;
}
