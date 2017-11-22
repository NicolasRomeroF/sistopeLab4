#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned char fileMarker1;                                                                                                                                                                                              
    unsigned char fileMarker2;                                                                                                                                                                                               
    unsigned int bfSize;                                                                                                                                                                                                                   
    uint16_t unused1;                                                                                                                                                                                                                        
    uint16_t unused2;                                                                                                                                                                                                                        
    unsigned int imageDataOffset;                                                                                                                                                            
} FILEHEADER;                                                                                                                                                                                                                                

typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned int biSize;                                                                                                                                                                                                                   
    int width;                                                                                                                                                          
    int height;                                                                                                                                                                     
    uint16_t planes;                                                                                                                                                                                                                         
    uint16_t bitPix;                                                                                                                                                                                                                         
    unsigned int biCompression;                                                                                                                                                                                                            
    unsigned int biSizeImage;                                                                                                                                                                                                              
    int biXPelsPerMeter;                                                                                                                                                                                                          
    int biYPelsPerMeter;                                                                                                                                                                                                          
    unsigned int biClrUsed;                                                                                                                                                                                      
    unsigned int biClrImportant;                                                                                                                                                                                                           
} INFOHEADER;                                                                                                                                                                                                                                

typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned char b;                                                                                                                                                                                                                        
    unsigned char g;                                                                                                                                                                                                                        
    unsigned char r;                                                                                                                                                                                                                        
} IMAGE;

typedef struct matriz{
	IMAGE ** matriz;
	int x;
	int y;
} Matriz;

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
    FILE *filePtr; //our file pointer
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    unsigned char *bitmapImage;  //store image data
    int imageIdx=0;  //image index counter
    unsigned char tempRGB;  //our swap variable

    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
    {
    	printf("ERROR: No se pudo abrir la imagen dada\n");
        return NULL;
    }

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(filePtr);
        printf("ERROR: La imagen no es de tipo bmp\n");
        return NULL;
    }

    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr); // small edit. forgot to add the closing bracket at sizeof

    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //verify memory allocation
    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(filePtr);
        printf("ERROR: No se pudo asignar suficiente memoria a la imagen\n");
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage,bitmapInfoHeader->biSizeImage,filePtr);

    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        fclose(filePtr);
        printf("ERROR: No se puedo leer la informacion de la imagen\n");
        return NULL;
    }

    //swap the r and b values to get RGB (bitmap is BGR)
    for (imageIdx = 0;imageIdx < bitmapInfoHeader->biSizeImage;imageIdx+=3) // fixed semicolon
    {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }

    //close file and return bitmap iamge data
    fclose(filePtr);
    return bitmapImage;
}






















void insertarPixel(IMAGE * pixeles, int n, int x, int y, Matriz * matrizFinal){
	int i,sumaR=0,sumaG=0,sumaB=0;
	for(i = 0;i<n;i++){
		sumaR=sumaR+pixeles[i].r;
		sumaG=sumaG+pixeles[i].g;
		sumaB=sumaB+pixeles[i].b;
	}
	matrizFinal->matriz[x][y].r=sumaR/n;
	matrizFinal->matriz[x][y].g=sumaG/n;
	matrizFinal->matriz[x][y].b=sumaB/n;
}


IMAGE* completarArregloPixeles(IMAGE * pixeles, int ultimaPos, int n){
	int i;
	for(i=ultimaPos;i<n;i++){
		pixeles[i].r=0;
		pixeles[i].g=0;
		pixeles[i].b=0;
	}
	return pixeles;
}

void reducirPorFilas(Matriz * matriz, Matriz * matrizFinal, int nPixeles){
	int i,j,z;
	IMAGE * pixeles=(IMAGE*)malloc(sizeof(IMAGE)*nPixeles);
	for(i=0;i<matriz->x;i++){
		for(j=0;j<matriz->y;j++){
			for(z=0;z<nPixeles;z++){
				pixeles[z].r=matriz->matriz[i][j+z].r;
				pixeles[z].g=matriz->matriz[i][j+z].g;
				pixeles[z].b=matriz->matriz[i][j+z].b;
				if(j+z>=matriz->y){
					pixeles=completarArregloPixeles(pixeles,j+z,nPixeles);
					break;
				}
			}
			insertarPixel(pixeles,nPixeles,matriz->x,matriz->y,matrizFinal);
			j=j+nPixeles;
		}
	}
}


int main()
{
	BITMAPINFOHEADER bitmapInfoHeader;
	unsigned char *bitmapData;
	bitmapData = LoadBitmapFile(".bmp",&bitmapInfoHeader);
	
	int i =0;
	for(i=0;i<100;i++)
	{
		printf("%c\n",bitpamData[i];
	}
	return 0;
//now do what you want with it, later on i will show you how to
}
