#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "funciones.h"

#pragma pack(push, 1)


void insertarPixelFila(IMAGE * pixeles, int n, int x, int y, Matriz * matrizFinal) {
	int i, sumaR = 0, sumaG = 0, sumaB = 0;
	//printf("Insertando pixel, n=%d\n", n);
	for (i = 0; i < n; i++) {
		sumaR = sumaR + pixeles[i].r;
		sumaG = sumaG + pixeles[i].g;
		sumaB = sumaB + pixeles[i].b;
	}
	matrizFinal->matriz[x][y].r = sumaR / n;
	matrizFinal->matriz[x][y].g = sumaG / n;
	matrizFinal->matriz[x][y].b = sumaB / n;
	//printf("		x=%d, y = %d\n", x, y);
	//printf("		Promedios: R = %d , G = %d, B = %d \n", matrizFinal->matriz[x][y].r, matrizFinal->matriz[x][y].g, matrizFinal->matriz[x][y].b);
}

void insertarPixelFilaImparLimite(IMAGE * pixeles, int n, int x, int y, Matriz * matrizFinal, int start, int nPixeles) {
	int i, sumaR = 0, sumaG = 0, sumaB = 0;
	//printf("Insertando pixel, n=%d, start = %d\n", n, start);
	for (i = start; i < nPixeles; i++) {
		sumaR = sumaR + pixeles[i].r;
		sumaG = sumaG + pixeles[i].g;
		sumaB = sumaB + pixeles[i].b;
	}
	matrizFinal->matriz[x][y].r = sumaR / n;
	matrizFinal->matriz[x][y].g = sumaG / n;
	matrizFinal->matriz[x][y].b = sumaB / n;
	//printf("		x=%d, y = %d\n", x, y);
	//printf("		Promedios: R = %d , G = %d, B = %d \n", matrizFinal->matriz[x][y].r, matrizFinal->matriz[x][y].g, matrizFinal->matriz[x][y].b);
}

void printMatriz(Matriz * matriz) {
	int i, j;
	for (i = 0; i < matriz->x; i++) {
		for (j = 0; j < matriz->y; j++) {
			//printf("[R=%d G=%d B=%d] ", matriz->matriz[i][j].r, matriz->matriz[i][j].g, matriz->matriz[i][j].b);
		}
		//printf("\n");
	}
}
void printArregloPixeles(IMAGE * pixeles, int nPixeles) {
	int i;
	for (i = 0; i < nPixeles; i++) {
		//printf("%d.- R = %d G = %d B = %d\n", i, pixeles[i].r, pixeles[i].g, pixeles[i].b);
	}
}

void reducirPorFilasPares(Matriz * matriz, Matriz * matrizFinal, int nPixeles) {
	int i, j, z;
	int isBorde = 0;
	IMAGE * pixeles = (IMAGE*)malloc(sizeof(IMAGE) * nPixeles);
	for (i = 0; i < matriz->x; i = i + 2) {
		for (j = 0; j < matriz->y; j++) {
			isBorde = 0;
			//printf("i=%d, j=%d\n", i, j);
			for (z = 0; z < nPixeles; z++) {
				//printf("	z=%d\n", z);
				pixeles[z].r = matriz->matriz[i][j + z].r;
				pixeles[z].g = matriz->matriz[i][j + z].g;
				pixeles[z].b = matriz->matriz[i][j + z].b;
				//printf("	pixel r=%d\n        pixel g = %d\n        pixel b = %d\n", pixeles[z].r, pixeles[z].g, pixeles[z].b);
				if (j + z >= matriz->y - 1 && nPixeles - z != 1) {
					//printf("Fuera de rango!, condicion de if = %d\n", j + z);
					isBorde = 1;
					insertarPixelFila(pixeles, z + 1, i, j / nPixeles, matrizFinal);
					////printArregloPixeles(pixeles,nPixeles);
					break;
				}
			}
			if (!isBorde) {
				insertarPixelFila(pixeles, nPixeles , i, j / nPixeles, matrizFinal);
			}
			j = j + nPixeles - 1;
			////printMatriz(matrizFinal);
		}
	}
	free(pixeles);
}

void reducirPorFilasImpares(Matriz * matriz, Matriz * matrizFinal, int nPixeles) {
	int i, j, z;
	int isBorde = 0;
	IMAGE * pixeles = (IMAGE*)malloc(sizeof(IMAGE) * nPixeles);
	for (i = 1; i < matriz->x; i = i + 2) {
		for (j = matriz->y - 1; j >= 0; j--) {
			isBorde = 0;
			//printf("i=%d,j=%d\n", i, j);
			for (z = nPixeles - 1; z >= 0; z--) {
				//printf("	z=%d\n", z);
				pixeles[z].r = matriz->matriz[i][j + (z - nPixeles + 1)].r;
				pixeles[z].g = matriz->matriz[i][j + (z - nPixeles + 1)].g;
				pixeles[z].b = matriz->matriz[i][j + (z - nPixeles + 1)].b;
				//printf("	pixel r=%d\n        pixel g = %d\n        pixel r = %d\n", pixeles[z].r, pixeles[z].g, pixeles[z].b);
				if ((j + (z - nPixeles + 1)) <= 0 && z != 0) {
					//printf("	Fuera de rango!, condicion de if = %d\n", (j + (z - nPixeles + 1)));
					isBorde = 1;
					//printf("VAlor de z antes de insertar en limite ********************************* %d\n", z);
					insertarPixelFilaImparLimite(pixeles, nPixeles - z, i, j / nPixeles, matrizFinal, z, nPixeles);
					break;
				}
			}
			if (!isBorde) {
				insertarPixelFila(pixeles, nPixeles, i, j / nPixeles, matrizFinal);
			}
			j = j - nPixeles + 1;
			////printMatriz(matrizFinal);
		}
	}
	free(pixeles);
}

void insertarPixelColumna(IMAGE * pixeles, int n, int x, int y, Matriz * matrizFinal) {
	int i, sumaR = 0, sumaG = 0, sumaB = 0;
	for (i = 0; i < n; i++) {
		sumaR = sumaR + pixeles[i].r;
		sumaG = sumaG + pixeles[i].g;
		sumaB = sumaB + pixeles[i].b;
	}
	matrizFinal->matriz[x][y].r = sumaR / n;
	matrizFinal->matriz[x][y].g = sumaG / n;
	matrizFinal->matriz[x][y].b = sumaB / n;
	//printf("		x=%d, y = %d\n", x, y);
	//printf("		Promedios: R = %d , G = %d, B = %d \n", matrizFinal->matriz[x][y].r, matrizFinal->matriz[x][y].g, matrizFinal->matriz[x][y].b);
}

void reducirPorColumnas(Matriz * matriz, Matriz * matrizFinal, int nPixeles) {
	int i, j, z;
	int isBorde = 0;
	IMAGE * pixeles = (IMAGE*)malloc(sizeof(IMAGE) * nPixeles);
	for (j = 0; j < matriz->y; j++) {
		for (i = 0; i < matriz->x; i++) {
			isBorde = 0;
			//printf("i=%d,j=%d\n", i, j);
			for (z = 0; z < nPixeles; z++) {
				pixeles[z].r = matriz->matriz[i + z][j].r;
				pixeles[z].g = matriz->matriz[i + z][j].g;
				pixeles[z].b = matriz->matriz[i + z][j].b;
				//printf("	pixel r=%d\n        pixel g = %d\n        pixel r = %d\n", pixeles[z].r, pixeles[z].g, pixeles[z].b, i + z);
				if ((i + z) >= matriz->x - 1) {
					//printf("	Fuera de rango!\n");
					isBorde = 1;
					insertarPixelColumna(pixeles, z + 1, i / nPixeles, j, matrizFinal);
					break;
				}
			}
			if (!isBorde) {
				insertarPixelColumna(pixeles, nPixeles, i / nPixeles, j, matrizFinal);
			}
			i = i + nPixeles - 1;
			////printMatriz(matrizFinal);
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

	//FILE *s = fopen(strcat(nombre, "prueba.bmp"), "wb");
	fread(&fh, sizeof(unsigned char), sizeof(FILEHEADER), img);
	fread(&ih, sizeof(unsigned char), sizeof(INFOHEADER), img);
	//fwrite(&fh, sizeof(unsigned char), sizeof(FILEHEADER), s);
	//fwrite(&ih, sizeof(unsigned char), sizeof(INFOHEADER), s);
	printf("%d+%d",sizeof(FILEHEADER),sizeof(INFOHEADER));

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
	unsigned int pixelBytesPerRow = ih.width*3;
	unsigned int paddingBytesPerRow = (4-(pixelBytesPerRow%4))%4;
	fseek(img, fh.imageDataOffset, SEEK_SET);
	IMAGE aux;
	//printf("asdsadn\n");

	int cont =0;
	//static unsigned char zeroes[3] = {0,0,0};

	for (i = ih.height - 1; i > -1; i--)
	{
		for (j = 0; j < ih.width; j++)
		{

			fread(&aux, sizeof(unsigned char), sizeof(IMAGE), img);
			im[i][j] = aux;
			//fwrite(&aux, sizeof(unsigned char), sizeof(IMAGE), s);

			cont++;

		}
		//fwrite(,sizeof(unsigned char),paddingBytesPerRow,s);
	}
printf("redi %d\n",cont);

	*fho = fh;
	*iho = ih;
	fclose(img);
	//fclose(s);
	return im;
}

Matriz* obtenerMatriz(char* nombre)
{
	FILEHEADER fh;
	INFOHEADER ih;
	IMAGE** img = obtenerImagen(nombre, &fh, &ih);
	if (img == NULL) {
		return NULL;
	}
	Matriz* m = (Matriz*)malloc(sizeof(Matriz));
	if (m == NULL) {
		return NULL;
	}
	m->matriz = img;
	m->x = ih.height;
	m->y = ih.width;
	return m;
}

void reducirPorFilas(Matriz * matriz, Matriz * matrizFinal, int nPixeles) {
	//printf("APLICANDO REDUCCION nPixeles = %d\n", nPixeles);
	reducirPorFilasPares(matriz, matrizFinal, nPixeles);
	reducirPorFilasImpares(matriz, matrizFinal, nPixeles);
	////printMatriz(matrizFinal);
	//printf("TERMINA REDUCCION\n");
}

void reducir(int nReducciones, int metodo, int nPixeles, char * entrada, FILE * salida1, FILE * salida2, int pantalla) {
	//printf("n iteraciones: %d\n", nReducciones);
	int i, j;
	/*int xM = 30, yM = 30;
	Matriz * matriz = (Matriz*)malloc(sizeof(Matriz));
	matriz->matriz = (IMAGE**)malloc(sizeof(IMAGE*)*xM);
	for (i = 0; i < xM; i++) {
		matriz->matriz[i] = (IMAGE*)malloc(sizeof(IMAGE) * yM);
	}
	for (i = 0; i < xM; i++) {
		for (j = 0; j < yM; j++) {
			matriz->matriz[i][j].r = rand() % 256;
			matriz->matriz[i][j].g = rand() % 256;
			matriz->matriz[i][j].b = rand() % 256;
		}
	}
	matriz->x = xM;
	matriz->y = yM;*/
	Matriz * matriz = obtenerMatriz(entrada);
	//guardarFichero(salida2, matriz);
	Matriz * matrizFinal;
	//printMatriz(matriz);
	if (metodo == 1) {
		int z = 0;
		while (z < nReducciones) {
			int i;
			//printf("*******************MATRIZ A REDUCIR**********************************\n");
			//printMatriz(matriz);
			//printf("nReducciones actual= %d----------------------------------------------------------------------\n", z);
			double div = ceil((double)matriz->y / nPixeles);
			int xMF = matriz->x, yMF = (int)div;
			//printf("nPixeles = %d, Dimensiones matriz final xMF= %d yMF = %d \n", nPixeles, xMF, yMF);
			matrizFinal = (Matriz*)malloc(sizeof(Matriz));
			matrizFinal->matriz = (IMAGE**)malloc(sizeof(IMAGE*)*xMF);
			for (i = 0; i < xMF; i++) {
				matrizFinal->matriz[i] = (IMAGE*)malloc(sizeof(IMAGE) * yMF);
			}
			for (i = 0; i < xMF; i++) {
				for (j = 0; j < yMF; j++) {
					matrizFinal->matriz[i][j].r = -1;
					matrizFinal->matriz[i][j].g = -1;
					matrizFinal->matriz[i][j].b = -1;
				}
			}
			matrizFinal->x = xMF;
			matrizFinal->y = yMF;
			reducirPorFilas(matriz, matrizFinal, nPixeles);
			matriz = matrizFinal;
			z++;
		}
		printf("fichero1\n");
		guardarFichero(salida1, matrizFinal);
		////printMatriz(matrizFinal);
	}
	else if (metodo == 2) {
		int z = 0;
		while (z < nReducciones) {
			int i;
			//printf("*******************MATRIZ A REDUCIR**********************************\n");
			//printMatriz(matriz);
			//printf("nReducciones actual= %d----------------------------------------------------------------------\n", z);
			double div = ceil((double)matriz->x / nPixeles);
			int yMF = matriz->y, xMF = (int)div;
			//printf("nPixeles = %d, Dimensiones matriz final xMF= %d yMF = %d \n", nPixeles, xMF, yMF);
			Matriz * matrizFinal = (Matriz*)malloc(sizeof(Matriz));
			matrizFinal->matriz = (IMAGE**)malloc(sizeof(IMAGE*)*xMF);
			for (i = 0; i < xMF; i++) {
				matrizFinal->matriz[i] = (IMAGE*)malloc(sizeof(IMAGE) * yMF);
			}
			for (i = 0; i < xMF; i++) {
				for (j = 0; j < yMF; j++) {
					matrizFinal->matriz[i][j].r = -1;
					matrizFinal->matriz[i][j].g = -1;
					matrizFinal->matriz[i][j].b = -1;
				}
			}
			matrizFinal->x = xMF;
			matrizFinal->y = yMF;
			reducirPorColumnas(matriz, matrizFinal, nPixeles);
			matriz = matrizFinal;
			z++;
		}
		printf("guaradndooo\n");
		guardarFichero(salida2,matrizFinal);
	}
}

int verificarEntradas(Matriz * fileEntrada, FILE * fileSalida1, FILE * fileSalida2, int nReducciones, int nPixeles, int metodo, int flag) {
	if (fileEntrada == NULL) {
		printf("No se pudo abrir archivo de entrada\n");
		return 0;
	}
	else if (fileSalida1 == NULL) {
		printf("No se pudo crear archivo de salida 1\n");
		return 0;
	}
	else if (fileSalida2 == NULL) {
		printf("No se pudo crear archivo de salida 2\n");
		return 0;
	}
	else if (nReducciones <= 0) {
		printf("La cantidad de reducciones debe ser mayor o igual a 1\n");
		return 0;
	}
	else if (nPixeles <= 1) {
		printf("La cantidad de pixeles debe ser mayor o igual a 2\n");
		return 0;
	}
	else if (metodo <= 0 || metodo >= 4) {
		printf("El método debe ser 1, 2 o 3\n");
		return 0;
	}
	return 1;
}

int init(int argc, char **argv) {
	/*
		-i: nombre del archivo de entrada.
		-s: nombre del archivo de salida para metodo uno.
		-g: nombre del archivo de salida para el segundo metodo.
		-n: cantidad de veces a implementar la reduccion.
		-m: cantidad de pixeles que se deben promediar.
		-o: metodo al cual se desea llamar.
		-d: bandera que indica si se deben mostrar los resultados por pantalla, es decir el tiempo de ejecucion
			del programa.
	*/

	//Variables de optarg
	char * archivoEntrada;
	char * archivoSalida1;
	char * archivoSalida2;
	int nReducciones;
	int nPixeles;
	int metodo;
	int flag;
	int c;
	int index;

	opterr = 0;

	//Con este ciclo se obtienen los argumentos ingresados por consola y se asignan
	//a las variables anteriormente declaradas
	while ((c = getopt (argc, argv, "i:s:g:n:m:o:d")) != -1) {
		switch (c) {
		case 'i':
			archivoEntrada = optarg;
			break;
		case 's':
			archivoSalida1 = optarg;
			break;
		case 'g':
			archivoSalida2 = optarg;
			break;
		case 'n':
			sscanf(optarg, "%d", &nReducciones);
			break;
		case 'm':
			sscanf(optarg, "%d", &nPixeles);
			break;
		case 'o':
			sscanf(optarg, "%d", &metodo);
			break;
		case 'd':
			sscanf(optarg, "%d", &flag);
			break;

		//Caso en que se ingrese una opcion desconocida
		case '?':
			if (optopt == 'i' || optopt == 's' || optopt == 'g' || optopt == 'n' || optopt == 'm' || optopt == 'o' || optopt == 'd') {
				fprintf (stderr, "Opcion -%c requiere un argumento.\n", optopt);
			}
			else if (isprint (optopt)) {
				fprintf (stderr, "Opcion desconocida `-%c'.\n", optopt);
			}
			else {
				fprintf (stderr, "Caracter desconocido `\\x%x'.\n", optopt);

			}
			return 1;
		default:
			return 1;
		}

	}
	//printf("i = %s s = %s g = %s n = %d m = %d o = %d d = %d", archivoEntrada, archivoSalida1, archivoSalida2, nReducciones, nPixeles, metodo, flag);


	//Se muestra mensaje en caso de ingresar un argumento que no existe
	for (index = optind; index < argc; index++) {
		//printf ("Argumento no existente %s\n", argv[index]);
		return -1;
	}

	FILE* fileSalida1 = fopen(archivoSalida1, "wb");
	FILE* fileSalida2 = fopen(archivoSalida2, "wb");


	if (verificarEntradas(obtenerMatriz(archivoEntrada), fileSalida1, fileSalida2, nReducciones, nPixeles, metodo, flag) == 0) {
		printf("salir\n");
		return 0;
	}
	printf("reducirrr\n");
	reducir(nPixeles, metodo, nReducciones, archivoEntrada, fileSalida1, fileSalida2, flag);
}

FILEHEADER* iniciarFileHeader()
{
	FILEHEADER* fh =(FILEHEADER*)malloc(sizeof(FILEHEADER));
	fh->fileMarker1 = 'B';
	fh->fileMarker2 = 'M';
	fh->bfSize = 0;
	fh->unused1 = 0;
	fh->unused2 = 0;
	fh->imageDataOffset = 54;
	return fh;
}

INFOHEADER* iniciarInfoHeader()
{
	INFOHEADER* ih=(INFOHEADER*)malloc(sizeof(INFOHEADER));
	ih->biSize = 40;
	ih->width = 0;
	ih->height = 0;
	ih->planes = 1;
	ih->bitPix = 24;
	ih->biCompression = 0;
	ih->biSizeImage = 0;
	ih->biXPelsPerMeter = 0;
	ih->biYPelsPerMeter = 0;
	ih->biClrUsed = 0;
	ih->biClrImportant = 0;
	return ih;
}

void writeMatriz(Matriz* m, FILE* f)
{
	IMAGE aux;
	int i, j;
	unsigned int pixelBytesPerRow = m->y*3;
	unsigned int paddingBytesPerRow = (4-(pixelBytesPerRow%4))%4;
	static unsigned char zeroes[3] = {0,0,0};

	for (i = m->x - 1; i > -1; i--)
	{
		for (j = 0; j < m->y; j++)
		{
			aux = m->matriz[i][j];
			fwrite(&aux, sizeof(unsigned char), sizeof(IMAGE), f);
		}
		fwrite(zeroes,sizeof(unsigned char),paddingBytesPerRow,f);
	}
}

int guardarFichero(FILE* f, Matriz* m)
{
	printf("entreGuardar\n");
	FILEHEADER* fh = iniciarFileHeader();
	printf("pasefileh\n");
	INFOHEADER* ih = iniciarInfoHeader();
	printf("paseih\n");
	printf("%dx%d\n",m->y,m->x);
	ih->width = m->y;

	ih->height = m->x;
	printf("%dx%d\n",m->y,m->x);
	int imageSize = m->y * m->x;
	int file_size = 54 + 4 * imageSize;
	fh->bfSize = file_size;
	ih->biSizeImage = imageSize;
	fwrite(&fh, sizeof(unsigned char), sizeof(FILEHEADER), f);
	fwrite(&ih, sizeof(unsigned char), sizeof(INFOHEADER), f);
	printf("escribir matriz\n");
	writeMatriz(m, f);
}

int main(int argc, char **argv)
{
	init(argc, argv);
}