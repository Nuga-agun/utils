#include <stdio.h>
#include <stdlib.h>
#include "lib/lodepng.h"

#ifndef END_BYTE
#define END_BYTE 2
#endif

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage : text_to_png [INPUT_PNG]\n");
		return EXIT_FAILURE;
	}
	
	const char* input_png = argv[1];
	unsigned char* image;
	unsigned width, height;

	unsigned error = lodepng_decode32_file(&image, &width, &height, input_png);
	if (error) {
		printf("Erreur de lecture PNG: %s\n", lodepng_error_text(error));
		return 1;
	}

	size_t nb_pixels = width * height;
	unsigned char* output = malloc(nb_pixels/8+1);
	size_t output_index = 0;
	unsigned char current_byte = 0;
	int bit_counter = 0;

	for (size_t i = 0; i < nb_pixels; i++) {
		unsigned char alpha = image[i*4 + 3];
		if (alpha == END_BYTE) break;
		current_byte = (current_byte << 1) | (alpha&1);
		bit_counter++;

		if (bit_counter == 8) {
			output[output_index++] = current_byte;
			current_byte = 0;
			bit_counter = 0;
		}
	}
	output[output_index] = EOF;

	printf("\n%s\n", output);

	free(output);
	free(image);
	return EXIT_SUCCESS;
}
