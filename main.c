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


IMAGE* completarArregloPixelesPares(IMAGE * pixeles, int y, int n){
	int i;
	for(i=y;i<n;i++){
		pixeles[i].r=0;
		pixeles[i].g=0;
		pixeles[i].b=0;
	}
	return pixeles;
}

IMAGE* completarArregloPixelesImpares(IMAGE * pixeles,int nFaltante, int n){
	int i;
	for(i=nFaltante;i<n;i++){
		pixeles[i].r=0;
		pixeles[i].g=0;
		pixeles[i].b=0;
	}
	return pixeles;
}

void reducirPorFilasPares(Matriz * matriz, Matriz * matrizFinal, int nPixeles){
	int i,j,z;
	IMAGE * pixeles=(IMAGE*)malloc(sizeof(IMAGE)*nPixeles);
	for(i=0;i<matriz->x;i=i+2){
		for(j=0;j<matriz->y;j++){
			for(z=0;z<nPixeles;z++){
				pixeles[z].r=matriz->matriz[i][j+z].r;
				pixeles[z].g=matriz->matriz[i][j+z].g;
				pixeles[z].b=matriz->matriz[i][j+z].b;
				if(j+z>=matriz->y){
					pixeles=completarArregloPixelesPares(pixeles,j+z,nPixeles);
					break;
				}
			}
			insertarPixel(pixeles,nPixeles,matriz->x,matriz->y,matrizFinal);
			j=j+nPixeles;
		}
	}
}

void reducirPorFilasImpares(Matriz * matriz, Matriz * matrizFinal, int nPixeles){
	int i,j,z;
	IMAGE * pixeles=(IMAGE*)malloc(sizeof(IMAGE)*nPixeles);
	for(i=1;i<matriz->x;i=i+2){
		for(j=matriz->y-1;j>=0;j--){
			for(z=nPixeles-1;z=>0;z--){
				pixeles[z].r=matriz->matriz[i][j+(z-nPixeles+1)].r;
				pixeles[z].g=matriz->matriz[i][j+(z-nPixeles+1)].g;
				pixeles[z].b=matriz->matriz[i][j+(z-nPixeles+1)].b;
				if(j-z=0){
					pixeles=completarArregloPixelesImpares(pixeles,nPixeles-z,nPixeles);
					break;
				}
			}
			insertarPixel(pixeles,nPixeles,matriz->x,matriz->y,matrizFinal);
			j=j-nPixeles;
		}
	}
}




int main() {                                                                                                                                                                                                                             
    FILEHEADER fh;                                                                                                                                                                                                                           
    INFOHEADER ih;                                                                                                                                                                                                                           
    FILE *img = fopen("wea4.bmp", "rb");
    fread(&fh, sizeof(unsigned char), sizeof(FILEHEADER), img);
    fread(&ih, sizeof(unsigned char), sizeof(INFOHEADER), img);
    printf("fM1 = %c, fM2 = %c, bfS = %u, un1 = %hu, un2 = %hu, iDO = %u\n", fh.fileMarker1, fh.fileMarker2, fh.bfSize, fh.unused1, fh.unused2, fh.imageDataOffset);                                                                         
    printf("w = %d, h = %d, biSI = %d\n", ih.width, ih.height, ih.biSizeImage);
    int i;
    
    IMAGE im;
    for(i=0;i<9;i++)
    {
    	fread(&im, sizeof(unsigned char), sizeof(IMAGE), img);
    	printf("r = %d, g = %d, b = %d\n", im.r, im.g, im.b);
    }
   return 0;
}

























