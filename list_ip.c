#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char **argv) {
	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *next_res;
	char buffer[INET6_ADDRSTRLEN];
	
	if (argc != 2) {
		printf("Usage : ./list_ip hostname\n");
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	int status = getaddrinfo(argv[1], 0, &hints, &res);
	if (status != 0) {
		printf("getaddrinfo failed : %s\n", gai_strerror(status));
		return EXIT_FAILURE;
	}
	
	printf("IP addresses for %s : \n", argv[1]);

	next_res = res;
	while (next_res != NULL) {
		void *addr;
		if (next_res->ai_family == AF_INET) {
			struct sockaddr_in *addr_ipv4 = (struct sockaddr_in *)next_res->ai_addr;
			inet_ntop(next_res->ai_family, &(addr_ipv4->sin_addr), buffer, sizeof buffer);
			printf("IPV4 addr: %s\n", buffer);
		} else {
			struct sockaddr_in6 *addr_ipv6 = (struct sockaddr_in6 *)next_res->ai_addr;
			inet_ntop(next_res->ai_family, &(addr_ipv6->sin6_addr), buffer, sizeof buffer);
			printf("IPV6 addr: %s\n", buffer);
		}
		next_res = next_res->ai_next;
	}
	freeaddrinfo(res);

	return EXIT_SUCCESS;
}
