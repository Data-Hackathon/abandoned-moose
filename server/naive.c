#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define BACKLOG 10
#define NMAX 1024

struct commands {
	char *name;
	void (*func)(char *);
};

void reverse(char *data) {
	char tmp, *p;
	if(data == NULL || !(*data))
		return;
	p = data + strlen(data) - 1;
	while(p > data) {
		tmp = *data;
		*data = *p;
		*p = tmp;
		data++;
		p--;
	}
}

int con(char *host, char *port) {
	int srv, yes = 1;
	struct addrinfo hints, *res, *r;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if(getaddrinfo(host, port, &hints, &res) != 0) {
		fprintf(stderr, "Error: cannot resolve hostname %s\n", host);
		exit(1);
	}
	for(r = res; r; r = r->ai_next) {
		if((srv = socket(r->ai_family, r->ai_socktype, r->ai_protocol)) < 0)
			continue;

		setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		
		if(bind(srv, res->ai_addr, res->ai_addrlen) == -1) {
			close(srv);
			fprintf(stderr, "Error binding!\n");
			continue;
		}
		break;
	}
	freeaddrinfo(res);
	if(!r) {
		fprintf(stderr, "Error: cannt connect to host %s\n", host);
		exit(1);
	}
	return srv;
}

int main() {
	int s, inc, numfds;
	char *host = "127.0.0.1";
	char *port = "9001";
	struct sockaddr_storage sinc;
	struct commands cmd[] = {{ "reverse", reverse}};
	fd_set md, rd;
	
	s = con(host, port);

	if(listen(s, BACKLOG) == -1) {
		close(s);
		fprintf(stderr, "Error listening!\n");
		return 1;
	}

	numfds = s;

	FD_ZERO(&md);
	FD_ZERO(&rd);
	FD_SET(s, &md);

	while(1) {
		int msglen, i;
		socklen_t addr_len;
		char *data = malloc(sizeof(char) * NMAX);
		rd = md;

		if(select(numfds+1, &rd, 0, 0, 0) == -1) {
			fprintf(stderr, "Error in select()\n");
			return 1;
		}

		for(i = 0; i <= numfds; i++) {
			if(FD_ISSET(i, &rd)) {
				if(i == s) { /* New connection */
					addr_len = sizeof sinc;
					if((inc = accept(s, (struct sockaddr *)&sinc, &addr_len)) == -1) {
						fprintf(stderr, "Error in accept()\n");
					}
					else {
						FD_SET(inc, &md);
						if(inc > numfds)
							numfds = inc;
						printf("Connected by someone!\n");
					}
				}
				else { /* Old connection, handle data */
					if((msglen = recv(i, data, NMAX-1, 0)) <= 0) {
						if(msglen == 0)
							fprintf(stderr, "Socket %d disconnected.\n", i);
						else
							fprintf(stderr, "Error in recv()\n");
						close(i);
						FD_CLR(i, &md);
					}
					else {
						char *index;
						int j;
						data[msglen] = '\0';
						printf("Got data: %s\nProcessing and sending...\n", data);

						if((index = strchr(data, ';')) != NULL) {
							for(j = 0; j < sizeof(cmd)/sizeof(cmd[0]); j++) {
								if(!strncmp(data, cmd[j].name, index-data)) {
									data = index + 1;
									cmd[j].func(data);
								}
							}
							/* This needs to be taken care of better in
							** the client.
							*/
							if(!strncmp(data, "broadcast", index-data)) {
								printf("Sending broadcast...\n");
								data = index + 1;
								for(j = 0; j < numfds; j++) {
									if(FD_ISSET(j, &md)) {
										if(j != s && j != i) {
											/* Sending broadcast to everyone
											** except myself and server
											*/
											send(j, data, strlen(data), 0);
										}
									}
								}
							}
						}
						send(i, data, strlen(data), 0);
					}
				}
			}
		}
	}

	close(s);

	return 0;
}
