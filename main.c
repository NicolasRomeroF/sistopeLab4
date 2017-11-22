#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#pragma pack(push, 1)

typedef struct {                                                                                                                                                                                                                             
    unsigned char fileMarker1;                                                                                                                                                                                              
    unsigned char fileMarker2;                                                                                                                                                                                               
    unsigned int bfSize;                                                                                                                                                                                                                   
    uint16_t unused1;                                                                                                                                                                                                                        
    uint16_t unused2;                                                                                                                                                                                                                        
    unsigned int imageDataOffset;                                                                                                                                                            
} FILEHEADER;                                                                                                                                                                                                                                

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct {                                                                                                                                                                                                                             
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

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct {                                                                                                                                                                                                                             
    unsigned char b;                                                                                                                                                                                                                        
    unsigned char g;                                                                                                                                                                                                                        
    unsigned char r;                                                                                                                                                                                                                        
} IMAGE;

#pragma pack(pop)

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
			for(z=nPixeles-1;z>=0;z--){
				pixeles[z].r=matriz->matriz[i][j+(z-nPixeles+1)].r;
				pixeles[z].g=matriz->matriz[i][j+(z-nPixeles+1)].g;
				pixeles[z].b=matriz->matriz[i][j+(z-nPixeles+1)].b;
				if(j-z==0){
					pixeles=completarArregloPixelesImpares(pixeles,nPixeles-z,nPixeles);
					break;
				}
			}
			insertarPixel(pixeles,nPixeles,matriz->x,matriz->y,matrizFinal);
			j=j-nPixeles;
		}
	}
}

IMAGE** obtenerImagen(char* nombre, FILEHEADER* fho, INFOHEADER* iho)
{
	FILEHEADER fh;                                                                                                                                                                                                                           
    INFOHEADER ih;                                                                                                                                                                                                                           
    FILE *img = fopen(nombre, "rb");
    if(img==NULL)
    {
    	printf("ERROR: No se pudo abrir la imagen\n");
    	return NULL;
    }
                                                                                                                                                                                                                        

    fread(&fh, sizeof(unsigned char), sizeof(FILEHEADER), img);
    fread(&ih, sizeof(unsigned char), sizeof(INFOHEADER), img);
    printf("fM1 = %c, fM2 = %c, bfS = %u, un1 = %hu, un2 = %hu, iDO = %u\n", fh.fileMarker1, fh.fileMarker2, fh.bfSize, fh.unused1, fh.unused2, fh.imageDataOffset);                                                                         
    printf("w = %d, h = %d, biSI = %d\n", ih.width, ih.height, ih.biSizeImage);
    int i,j;
    
    IMAGE** im=(IMAGE**)malloc(sizeof(IMAGE*)*ih.height);
    if(im==NULL)
    {
    	printf("ERROR: No se pudo asignar memoria a la imagen\n");
    	return NULL;
    }
    for(i=0;i<ih.height;i++)
    {
    	im[i]=(IMAGE*)malloc(sizeof(IMAGE)*ih.width);
    	if(im[i]==NULL)
	    {
	    	printf("ERROR: No se pudo asignar memoria a la imagen\n");
	    	return NULL;
	    }
    }
    fseek(img, fh.imageDataOffset, SEEK_SET);
    IMAGE aux;
    printf("asdsadn\n");
    /*for(i=ih.height-1;i>-1;i--)
    {
    	for(j=0;j<ih.width;j++)
    	{
    		
    		fread(&aux, sizeof(unsigned char), sizeof(IMAGE), img);
    		//printf("r = %d, g = %d, b = %d\n", im[i][j].r, im[i][j].g, im[i][j].b);
    	}
    }*/

    for(i=ih.height-1;i>-1;i--)
    {
    	for(j=0;j<ih.width;j++)
    	{
    		
    		fread(&aux, sizeof(unsigned char), sizeof(IMAGE), img);
    		//printf("ra = %d, ga = %d, ba = %d\n", aux.r, aux.g, aux.b);
    		im[i][j]=aux;
    		//printf("rm = %d, gm = %d, bm = %d\n", im[i][j].r, im[i][j].g, im[i][j].b);
    	}
    }

    for(j=0;j<5;j++)
    {
		printf("r = %d, g = %d, b = %d\n", im[0][j].r, im[0][j].g, im[0][j].b);
    }
    *fho=fh;
    *iho=ih;
    return im;
}

int main()
{
    FILEHEADER fh;                                                                                                                                                                                                                           
    INFOHEADER ih; 
    IMAGE** img = obtenerImagen("cuadro.bmp",&fh,&ih);

}




























