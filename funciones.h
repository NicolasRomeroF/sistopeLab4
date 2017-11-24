#ifndef _FUNCIONES_H_
#define _FUNCIONES_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <ctype.h>

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

void insertarPixelFila(IMAGE * pixeles, int n, int x, int y, Matriz * matrizFinal);

void insertarPixelFilaImparLimite(IMAGE * pixeles, int n, int x, int y, Matriz * matrizFinal, int start, int nPixeles);

void printMatriz(Matriz * matriz);

void printArregloPixeles(IMAGE * pixeles, int nPixeles);

void reducirPorFilasPares(Matriz * matriz, Matriz * matrizFinal, int nPixeles);

void reducirPorFilasImpares(Matriz * matriz, Matriz * matrizFinal, int nPixeles);

void insertarPixelColumna(IMAGE * pixeles, int n, int x, int y, Matriz * matrizFinal);

void reducirPorColumnas(Matriz * matriz, Matriz * matrizFinal, int nPixeles);

IMAGE** obtenerImagen(char* nombre, FILEHEADER* fho, INFOHEADER* iho);

IMAGE** obtenerImagen(char* nombre, FILEHEADER* fho, INFOHEADER* iho);


void reducirPorFilas(Matriz * matriz, Matriz * matrizFinal, int nPixeles);

void reducir(int nReducciones, int metodo, int nPixeles, char * entrada, FILE * salida1, FILE * salida2, int pantalla);

int verificarEntradas(Matriz * fileEntrada, FILE * fileSalida1, FILE * fileSalida2, int nReducciones, int nPixeles, int metodo, int flag);

int init(int argc, char **argv);

FILEHEADER* iniciarFileHeader();

INFOHEADER* iniciarInfoHeader();

void writeMatriz(Matriz* m, FILE* f);

int guardarFichero(FILE* f, Matriz* m,FILEHEADER fh,INFOHEADER ih);

int main(int argc, char **argv);

#endif