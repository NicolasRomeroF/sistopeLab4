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

typedef struct {
	INFOHEADER ih;
	FILEHEADER fh;
	IMAGE** img;
	Matriz* m;
} IMGINFO;

typedef struct BMP
{
	char bm[2];					//(2 Bytes) BM (Tipo de archivo)
	int tamano;					//(4 Bytes) Tamaño del archivo en bytes
	int reservado;					//(4 Bytes) Reservado
	int offset;						//(4 Bytes) offset, distancia en bytes entre la img y los píxeles
	int tamanoMetadatos;			//(4 Bytes) Tamaño de Metadatos (tamaño de esta estructura = 40)
	int alto;						//(4 Bytes) Ancho (numero de píxeles horizontales)
	int ancho;					//(4 Bytes) Alto (numero de pixeles verticales)
	short int numeroPlanos;			//(2 Bytes) Numero de planos de color
	short int profundidadColor;		//(2 Bytes) Profundidad de color (debe ser 24 para nuestro caso)
	int tipoCompresion;				//(4 Bytes) Tipo de compresión (Vale 0, ya que el bmp es descomprimido)
	int tamanoEstructura;			//(4 Bytes) Tamaño de la estructura Imagen (Paleta)
	int pxmh;					//(4 Bytes) Píxeles por metro horizontal
	int pxmv;					//(4 Bytes) Píxeles por metro vertical
	int coloresUsados;				//(4 Bytes) Cantidad de colores usados 
	int coloresImportantes;			//(4 Bytes) Cantidad de colores importantes
	IMAGE** pixel; 
	Matriz* m;			//Puntero a una tabla dinamica de caracteres de 2 dimenciones almacenara el valor del pixel en escala de gris (0-255)
}BMP;



void insertarPixelFila(IMAGE * pixeles, int n, int x, int y, Matriz * matrizFinal);
void insertarPixelFilaImparLimite(IMAGE * pixeles, int n, int x, int y, Matriz * matrizFinal, int start, int nPixeles);
void printMatriz(Matriz * matriz);
void printArregloPixeles(IMAGE * pixeles, int nPixeles);
void reducirPorFilasPares(Matriz * matriz, Matriz * matrizFinal, int nPixeles);
void reducirPorFilasImpares(Matriz * matriz, Matriz * matrizFinal, int nPixeles);
void insertarPixelColumna(IMAGE * pixeles, int n, int x, int y, Matriz * matrizFinal);
void reducirPorColumnas(Matriz * matriz, Matriz * matrizFinal, int nPixeles);
IMGINFO* obtenerImagen(char* nombre);
IMGINFO* obtenerMatriz(char* nombre);
void reducirPorFilas(Matriz * matriz, Matriz * matrizFinal, int nPixeles);
void abrir_imagen(BMP *imagen, char *ruta);
Matriz * copiarMatriz(Matriz * matriz);
void reducir(int nReducciones, int metodo, int nPixeles, char * entrada, FILE * salida1, FILE * salida2, int pantalla);
int verificarEntradas(IMGINFO * fileEntrada, FILE * fileSalida1, FILE * fileSalida2, int nReducciones, int nPixeles, int metodo, int flag);
int init(int argc, char **argv);
FILEHEADER* iniciarFileHeader();
INFOHEADER* iniciarInfoHeader();
void writeMatriz(Matriz* m, FILE* f);
int guardarFichero(FILE* archivo, Matriz* m, BMP* imagen);

#endif