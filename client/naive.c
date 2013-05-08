#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define HOST "127.0.0.1"
#define PORT 9001
#define NMAX 1024

int main() {
	int s;
	struct sockaddr_in dest;
	char msg[NMAX], data[NMAX];
	fd_set rd;
	/* TODO:
			use getaddrinfo!
	*/

	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		fprintf(stderr, "Error creating socket.\n");
		return 1;
	}

	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr(HOST);
	dest.sin_port = htons(PORT);

	if(connect(s, (struct sockaddr *)&dest, sizeof(struct sockaddr)) < 0) {
		fprintf(stderr, "Error: unable to connect.\n");
		return 1;
	}

	while(1) {
		int i;
		FD_ZERO(&rd);
		FD_SET(0, &rd); /* stdin */
		FD_SET(s, &rd);
		
		if((i = select(s + 1, &rd, 0, 0, 0)) == -1) {
			fprintf(stderr, "Error in select()\n");
			return 1;
		}
		if(FD_ISSET(s, &rd)) {
			int datalen;
			datalen = recv(s, data, NMAX, 0);
			data[datalen] = '\0';
			printf("got> %s\n", data);
		}
		if(FD_ISSET(0, &rd)) {
			if(fgets(msg, NMAX, stdin) == NULL)
				break;
			send(s, msg, strlen(msg) - 1, 0);
		}
	}
	close(s);

	return 0;
}
