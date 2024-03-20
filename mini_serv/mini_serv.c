#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_client {
	int id;
	char msg[1024];
}	t_client;

t_client clients[1024];
fd_set active, readfd, writefd;
int maxfd = 0, nextid = 0;
char buftoread[120000], buftowrite[120000];


void err(char *msg) {
	if (msg) {
		write(2, msg, strlen(msg));
	}
	else {
		write(2, "Fatal error", 11);
	}
	write(2, "\n", 1);
	exit(1);
}

void sendAll(int senderFd) {
	for (int fd = 0; fd <= maxfd; fd++) {
		if (FD_ISSET(fd, &writefd) && fd != senderFd) {
			send(fd, &buftowrite, strlen(buftowrite), 0);
		}
	}
}

int main(int argc, char **argv) {
	if (argc != 2) {
		err("Wrong number of arguments");
	}
	socklen_t len;
	struct sockaddr_in servaddr; 

	// socket create and verification 
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) err(NULL);
	FD_ZERO(&active);
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433);
	servaddr.sin_port = htons(atoi(argv[1])); 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) err(NULL);
	if (listen(sockfd, 10) != 0) err(NULL);
	FD_SET(sockfd, &active);
	maxfd = sockfd;
	int index = 0;
	while (1) {
		readfd = writefd = active;
		if (select(maxfd + 1, &readfd, &writefd, NULL, NULL) < 0) continue ;
		for (int fd = 0; fd <= maxfd; fd++) {
			if (FD_ISSET(fd, &readfd) && fd == sockfd) {
					int clientfd = accept(sockfd, (struct sockaddr *)&servaddr, &len);
					if (clientfd <= 0) continue ;
					clients[clientfd].id = nextid++;
					FD_SET(clientfd, &active);
					if (clientfd > maxfd) maxfd = clientfd;
					sprintf(buftowrite, "server: client %d just arrived\n", index -1);
					sendAll(clientfd);
					break;
				}
			if (FD_ISSET(fd, &readfd) && fd != sockfd) {
					int nbytes = recv(fd, buftoread, 70000, 0);
					if (nbytes <= 0) {
						FD_CLR(fd, &active);
						char buftowrite[100];
						sprintf(buftowrite, "server: client %d just left\n", clients[fd].id);
						sendAll(fd);
						close(fd);
						break;
					}
					else {
						for (int i = 0, j = strlen(clients[fd].msg); i < nbytes; i++, j++) {
							clients[fd].msg[j] = buftoread[i];
							if (clients[fd].msg[j] == '\n') {
								clients[fd].msg[j] = 0;
								sprintf(buftowrite, "client %d: %s\n", clients[fd].id, clients[fd].msg);
								sendAll(fd);
								bzero(clients[fd].msg, strlen(clients[fd].msg));
								j = -1;
						}
					}
				}
			}
		}
	}
}
