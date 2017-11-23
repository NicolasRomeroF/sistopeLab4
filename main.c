#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

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

typedef struct matriz {
    IMAGE ** matriz;
    int x;
    int y;
} Matriz;

void insertarPixelFila(IMAGE * pixeles, int n, int x, int y, Matriz * matrizFinal){
	int i,sumaR=0,sumaG=0,sumaB=0;
	printf("Insertando pixel, n=%d\n",n);
	for(i = 0;i<n;i++){
		sumaR=sumaR+pixeles[i].r;
		sumaG=sumaG+pixeles[i].g;
		sumaB=sumaB+pixeles[i].b;
	}
	matrizFinal->matriz[x][y].r=sumaR/n;
	matrizFinal->matriz[x][y].g=sumaG/n;
	matrizFinal->matriz[x][y].b=sumaB/n;
	printf("		x=%d, y = %d\n",x,y);
	printf("		Promedios: R = %d , G = %d, B = %d \n",matrizFinal->matriz[x][y].r,matrizFinal->matriz[x][y].g,matrizFinal->matriz[x][y].b);
}

void insertarPixelFilaImparLimite(IMAGE * pixeles, int n, int x, int y, Matriz * matrizFinal, int start){
	int i,sumaR=0,sumaG=0,sumaB=0;
	printf("Insertando pixel, n=%d, start = %d\n",n,start);
	for(i = start;i<=n;i++){
		sumaR=sumaR+pixeles[i].r;
		sumaG=sumaG+pixeles[i].g;
		sumaB=sumaB+pixeles[i].b;
	}
	matrizFinal->matriz[x][y].r=sumaR/n;
	matrizFinal->matriz[x][y].g=sumaG/n;
	matrizFinal->matriz[x][y].b=sumaB/n;
	printf("		x=%d, y = %d\n",x,y);
	printf("		Promedios: R = %d , G = %d, B = %d \n",matrizFinal->matriz[x][y].r,matrizFinal->matriz[x][y].g,matrizFinal->matriz[x][y].b);
}

void printMatriz(Matriz * matriz){
	int i,j;
	for(i=0;i<matriz->x;i++){
		for(j=0;j<matriz->y;j++){
			printf("[R=%d G=%d B=%d] ",matriz->matriz[i][j].r,matriz->matriz[i][j].g,matriz->matriz[i][j].b);
		}
		printf("\n");
	}
}
void printArregloPixeles(IMAGE * pixeles, int nPixeles){
	int i;
	for(i=0;i<nPixeles;i++){
		printf("%d.- R = %d G = %d B = %d\n",i,pixeles[i].r,pixeles[i].g,pixeles[i].b);
	}
}

void reducirPorFilasPares(Matriz * matriz, Matriz * matrizFinal, int nPixeles){
	int i,j,z;
	int isBorde=0;
	IMAGE * pixeles=(IMAGE*)malloc(sizeof(IMAGE)*nPixeles);
	for(i=0;i<matriz->x;i=i+2){
		for(j=0;j<matriz->y;j++){
			isBorde=0;
			printf("i=%d, j=%d\n",i,j);
			for(z=0;z<nPixeles;z++){
				printf("	z=%d\n",z);
				pixeles[z].r=matriz->matriz[i][j+z].r;
				pixeles[z].g=matriz->matriz[i][j+z].g;
				pixeles[z].b=matriz->matriz[i][j+z].b;
				printf("	pixel r=%d\n        pixel g = %d\n        pixel r = %d\n",pixeles[z].r,pixeles[z].g,pixeles[z].b);
				if(j+z>=matriz->y-1){
					isBorde=1;
					insertarPixelFila(pixeles,nPixeles-z,i,j/nPixeles,matrizFinal);
					printArregloPixeles(pixeles,nPixeles);
					break;
				}
			}
			if(!isBorde){
				insertarPixelFila(pixeles,nPixeles,i,j/nPixeles,matrizFinal);
			}
			j=j+nPixeles-1;
			printMatriz(matrizFinal);
		}
	}
	free(pixeles);
}

void reducirPorFilasImpares(Matriz * matriz, Matriz * matrizFinal, int nPixeles){
	int i,j,z;
	int isBorde=0;
	IMAGE * pixeles=(IMAGE*)malloc(sizeof(IMAGE)*nPixeles);
	for(i=1;i<matriz->x;i=i+2){
		for(j=matriz->y-1;j>=0;j--){
			isBorde=0;
			printf("i=%d,j=%d\n",i,j);
			for(z=nPixeles-1;z>=0;z--){
				printf("	z=%d\n",z);
				pixeles[z].r=matriz->matriz[i][j+(z-nPixeles+1)].r;
				pixeles[z].g=matriz->matriz[i][j+(z-nPixeles+1)].g;
				pixeles[z].b=matriz->matriz[i][j+(z-nPixeles+1)].b;
				printf("	pixel r=%d\n        pixel g = %d\n        pixel r = %d\n",pixeles[z].r,pixeles[z].g,pixeles[z].b);
				if((j+(z-nPixeles+1))<=0){
					printf("	Fuera de rango!\n");
					isBorde=1;
					insertarPixelFilaImparLimite(pixeles,nPixeles-z,i,j/nPixeles,matrizFinal,z);
					break;
				}
			}
			if(!isBorde){
				insertarPixelFila(pixeles,nPixeles,i,j/nPixeles,matrizFinal);
			}
			j=j-nPixeles+1;
			printMatriz(matrizFinal);
		}
	}
	free(pixeles);
}

void insertarPixelColumna(IMAGE * pixeles, int n, int x, int y, Matriz * matrizFinal){
	int i,sumaR=0,sumaG=0,sumaB=0;
	for(i=0;i<n;i++){
		sumaR=sumaR+pixeles[i].r;
		sumaG=sumaG+pixeles[i].g;
		sumaB=sumaB+pixeles[i].b;
	}
	matrizFinal->matriz[x][y].r=sumaR/n;
	matrizFinal->matriz[x][y].g=sumaG/n;
	matrizFinal->matriz[x][y].b=sumaB/n;
	printf("		x=%d, y = %d\n",x,y);
	printf("		Promedios: R = %d , G = %d, B = %d \n",matrizFinal->matriz[x][y].r,matrizFinal->matriz[x][y].g,matrizFinal->matriz[x][y].b);
}

void reducirPorColumnas(Matriz * matriz, Matriz * matrizFinal, int nPixeles){
	int i,j,z;
	int isBorde=0;
	IMAGE * pixeles=(IMAGE*)malloc(sizeof(IMAGE)*nPixeles);
	for(j=0;j<matriz->y;j++){
		for(i=0;i<matriz->x;i++){
			isBorde=0;
			printf("i=%d,j=%d\n",i,j);
			for(z=0;z<nPixeles;z++){
				pixeles[z].r=matriz->matriz[i+z][j].r;
				pixeles[z].g=matriz->matriz[i+z][j].g;
				pixeles[z].b=matriz->matriz[i+z][j].b;
				printf("	pixel r=%d\n        pixel g = %d\n        pixel r = %d\n",pixeles[z].r,pixeles[z].g,pixeles[z].b,i+z);
				if((i+z)>=matriz->x-1){
					printf("	Fuera de rango!\n");
					isBorde=1;
					insertarPixelColumna(pixeles,nPixeles-z,i/nPixeles,j,matrizFinal);
					break;
				}
			}
			if(!isBorde){
				insertarPixelColumna(pixeles,nPixeles,i/nPixeles,j,matrizFinal);
			}
			i=i+nPixeles-1;
			printMatriz(matrizFinal);
		}
	}
}

IMAGE** obtenerImagen(char* nombre, FILEHEADER* fho, INFOHEADER* iho)
{
    FILEHEADER fh;
    INFOHEADER ih;
    FILE *img = fopen(nombre, "rb");
    if (img == NULL)
    {
        printf("ERROR: No se pudo abrir la imagen\n");
        return NULL;
    }


    fread(&fh, sizeof(unsigned char), sizeof(FILEHEADER), img);
    fread(&ih, sizeof(unsigned char), sizeof(INFOHEADER), img);
    printf("fM1 = %c, fM2 = %c, bfS = %u, un1 = %hu, un2 = %hu, iDO = %u\n", fh.fileMarker1, fh.fileMarker2, fh.bfSize, fh.unused1, fh.unused2, fh.imageDataOffset);
    printf("w = %d, h = %d, biSI = %d\n", ih.width, ih.height, ih.biSizeImage);
    int i, j;

    IMAGE** im = (IMAGE**)malloc(sizeof(IMAGE*)*ih.height);
    if (im == NULL)
    {
        printf("ERROR: No se pudo asignar memoria a la imagen\n");
        return NULL;
    }
    for (i = 0; i < ih.height; i++)
    {
        im[i] = (IMAGE*)malloc(sizeof(IMAGE) * ih.width);
        if (im[i] == NULL)
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

    for (i = ih.height - 1; i > -1; i--)
    {
        for (j = 0; j < ih.width; j++)
        {

            fread(&aux, sizeof(unsigned char), sizeof(IMAGE), img);
            //printf("ra = %d, ga = %d, ba = %d\n", aux.r, aux.g, aux.b);
            im[i][j] = aux;
            //printf("rm = %d, gm = %d, bm = %d\n", im[i][j].r, im[i][j].g, im[i][j].b);
        }
    }


    *fho = fh;
    *iho = ih;
    return im;
}

Matriz* obtenerMatriz(char* nombre)
{
    FILEHEADER fh;
    INFOHEADER ih;
    IMAGE** img = obtenerImagen(nombre, &fh, &ih);
    Matriz* m = (Matriz*)malloc(sizeof(Matriz));
    m->matriz = img;
    m->x = ih.height;
    m->y = ih.width;
    return m;
}

int main()
{
	system("cls");
	int i,j;
	int xM=8,yM=8;
	int xMF=3,yMF=8;
	Matriz * matriz = (Matriz*)malloc(sizeof(Matriz));
	matriz->matriz = (IMAGE**)malloc(sizeof(IMAGE*)*xM);
	for(i=0;i<xM;i++){
		matriz->matriz[i]=(IMAGE*)malloc(sizeof(IMAGE)*yM);
	}
	for(i=0;i<xM;i++){
		for(j=0;j<yM;j++){
			matriz->matriz[i][j].r=rand() % 256;
			matriz->matriz[i][j].g=rand() % 256;
			matriz->matriz[i][j].b=rand() % 256;
		}
	}
	matriz->x=xM;
	matriz->y=yM;
	Matriz * matrizFinal = (Matriz*)malloc(sizeof(Matriz));
	matrizFinal->matriz = (IMAGE**)malloc(sizeof(IMAGE*)*xMF);
	for(i=0;i<xMF;i++){
		matrizFinal->matriz[i]=(IMAGE*)malloc(sizeof(IMAGE)*yMF);
	}
	for(i=0;i<xMF;i++){
		for(j=0;j<yMF;j++){
			matrizFinal->matriz[i][j].r=-1;
			matrizFinal->matriz[i][j].g=-1;
			matrizFinal->matriz[i][j].b=-1;
		}
	}
	matrizFinal->x=xMF;
	matrizFinal->y=yMF;
	printMatriz(matriz);
	//reducirPorFilasPares(matriz,matrizFinal,3);
	//reducirPorFilasImpares(matriz,matrizFinal,3);
	reducirPorColumnas(matriz,matrizFinal,3);
	printMatriz(matriz);
	printMatriz(matrizFinal);
}




























