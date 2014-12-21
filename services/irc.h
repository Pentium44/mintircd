#define SOCKBUFIN 2048

int irc_send(int socketfd, char *out);
int irc_connect(char *server, int port, int *socketfd);
int irc_read(int socketfd, char *in);
