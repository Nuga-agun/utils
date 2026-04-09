#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Usage : tcp_connector [ADDR] [PORT] [MESSAGE]\n");
		return EXIT_FAILURE;
	}

	const char *hostname = argv[1];
	const char *port_str = argv[2];
	const char *message = argv[3];

	int sock = 0;
	struct addrinfo hints, *res, *p;
	char buffer[1024] = {0};

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	int status;
	if ((status = getaddrinfo(hostname, port_str, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo error : %s\n", gai_strerror(status));
	}

	for (p = res; p!= NULL; p = p->ai_next) {
		if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
			continue;
		}
		if (connect(sock, p->ai_addr, p->ai_addrlen) < 0) {
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "Failed to connect\n");
		return EXIT_FAILURE;
	}

	freeaddrinfo(res);

	// Send message
	send(sock, message, strlen(message), 0);
	printf("Message sent: %s\n", message);

	// Read response
	size_t valread = read(sock, buffer, 1024);
	if (valread <= 0) {
		printf("No response received\n");
		return EXIT_FAILURE;
	}
	printf("Server response: %s\n", buffer);

	close(sock);
	return 0;
}
