#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 1500

int hex_to_bytes(const char *src, unsigned char *dest) {
	size_t len = strlen(src);
	if (len %2 != 0 || len > MAX_LEN) {
		printf("Erreur à la conversion en octets : chaîne trop longue ou problème de parité : %d\n", len);
		return -1;
	}
	for (size_t i = 0; i < len/2; i++) {
		sscanf(src+2*i, "%2hhx", &dest[i]);
	}
	return len/2;
}

int main (int argc, char **argv) {
	if (argc != 2) {
		printf("Usage : ./read_ethernet file.txt\n");
		return EXIT_FAILURE;
	}

	printf("[START]\n\n");
	FILE *filePtr;
	filePtr = fopen(argv[1], "r");
	char str_buffer[MAX_LEN];
	unsigned char bytes_buffer[MAX_LEN];
	fgets(str_buffer, MAX_LEN, filePtr);
	str_buffer[strcspn(str_buffer, "\n")] = 0;
	int len = hex_to_bytes(str_buffer, bytes_buffer);
	if (len < 0) {
		return EXIT_FAILURE;
	}
	if (len < 14) {
		printf("Erreur : trame trop courte.\n");
		return EXIT_FAILURE;
	}
	printf("Addresse destination : %02x:%02x:%02x:%02x:%02x:%02x\t", bytes_buffer[0], bytes_buffer[1], bytes_buffer[2], bytes_buffer[3], bytes_buffer[4], bytes_buffer[5]);
	printf("Addresse source : %02x:%02x:%02x:%02x:%02x:%02x\t", bytes_buffer[6], bytes_buffer[7], bytes_buffer[8], bytes_buffer[9], bytes_buffer[10], bytes_buffer[11]);
	printf("EtherType : 0x%02x%02x\n", bytes_buffer[12], bytes_buffer[13]);
	printf("Données : ");
	for (int i = 14; i<len; i++) {
		unsigned char next_byte = bytes_buffer[i];
		if (isprint(next_byte)) {
			putchar(next_byte);
		} else {
			putchar('.');
		}
	}
	fclose(filePtr);
	printf("\n[END]\n");
	return EXIT_SUCCESS;
}
