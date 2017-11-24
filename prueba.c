#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

//define pixelformat of windows bitmap, notice the unusual ordering of colors
typedef struct {
	unsigned char B;
	unsigned char G;
	unsigned char R;
} pixel;

//supply an array of pixels[height][width] <- notice that height comes first
int writeBMP(char* filename, unsigned int width, unsigned int height, pixel** pixels) {
	int fd = fopen(filename,"wb");  
	static unsigned char header[54] = {66,77,0,0,0,0,0,0,0,0,54,0,0,0,40,0,0,0,0,0,0,0,0,0,0,0,1,0,24}; //rest is zeroes
	unsigned int pixelBytesPerRow = width*sizeof(pixel);
	unsigned int paddingBytesPerRow = (4-(pixelBytesPerRow%4))%4;
	unsigned int* sizeOfFileEntry = (unsigned int*) &header[2];
	*sizeOfFileEntry = 54 + (pixelBytesPerRow+paddingBytesPerRow)*height;  
	unsigned int* widthEntry = (unsigned int*) &header[18];    
	*widthEntry = width;
	unsigned int* heightEntry = (unsigned int*) &header[22];    
	*heightEntry = height;
	write(fd, header, 54);
	static unsigned char zeroes[3] = {0,0,0}; //for padding    
	for (int row = 0; row < height; row++) {
		write(fd,pixels[row],pixelBytesPerRow);
		write(fd,zeroes,paddingBytesPerRow);
	}
	close(fd);
	return 0;
}