#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/lodepng.h"

#ifndef END_BYTE
#define END_BYTE 2
#endif

int main(int argc, char **argv) {
	if (argc != 4) {
		printf("Usage : text_to_png [INPUT_PNG] [OUTPUT_PNG] [TEXT_FILE]\n");
		return EXIT_FAILURE;
	}

	printf("Début de l'encodage ...\n");
	
	const char* input_png = argv[1];
	const char* output_png = argv[2];
	const char* text_file = argv[2];

	FILE *text_file_ptr;
	text_file_ptr = fopen(argv[3],"r");
	if (text_file_ptr == NULL) {
		printf("Error : unable to open the file %s\n", text_file);
		return EXIT_FAILURE;
	}

	unsigned char* image;
	unsigned width, height;
	unsigned error = lodepng_decode32_file(&image, &width, &height, input_png);
	if (error) {
		printf("Error while loading PNG %s : %s\n", input_png, lodepng_error_text(error));
		return EXIT_FAILURE;
	}

	fseek(text_file_ptr, 0L, SEEK_END);
	size_t text_size = ftell(text_file_ptr);
	rewind(text_file_ptr);
	size_t nb_pixels = width * height;
	if (nb_pixels*8 < text_size) {
		printf("The file to encode is too big. It will be truncated\nKeep going anyway ? (y/n) : ");
		char user_input = getchar();
		if (user_input != 'y') {
			printf("\nCancelling\n");
			fclose(text_file_ptr);
			free(image);
			return EXIT_SUCCESS;
		}
	}

	char next_char;
	size_t pixel_index = 0;
	while ((next_char = fgetc(text_file_ptr)) != EOF) {
		for (int i = 7; i >= 0; i--) { 
			unsigned bit = (next_char >> i) & 1;
			image[pixel_index * 4 + 3] = bit; // canal alpha
			pixel_index++;
		}
	}
	image[pixel_index*4 + 3] = END_BYTE;

	// Sauvegarde
	error = lodepng_encode32_file(output_png, image, width, height);
	if (error) {
		printf("Error while writing to PNG %s : %s\n", output_png, lodepng_error_text(error));
		return EXIT_FAILURE;
	}

	printf("Encodage terminé !\n");
	free(image);
	fclose(text_file_ptr);
	return 0;
}
