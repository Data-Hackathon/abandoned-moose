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


	while(printf("send> "), scanf("%s", msg) == 1) {
		int datalen;
		send(s, msg, strlen(msg), 0);
		datalen = recv(s, data, NMAX, 0);
		data[datalen] = '\0';
		printf("got> %s\n", data);
	}
	close(s);

	return 0;
}
