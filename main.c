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

/*Funcion que inserta los pixeles en la matriz modificada
  Entrada: Arreglo de pixeles a insertar, cantidad de pixeles, dimensiones de la matriz de salida,
           matriz de salida*/
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

/*Funcion que inserta los pixeles en la matriz modificada pero solo en el limita de una fila impar
  Entrada: Arreglo de pixeles a insertar, cantidad de pixeles, dimensiones de la matriz de salida,
           matriz de salida, posicion desde donde leer*/
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

/*Funcion que muestra una matriz por pantalla
  Entrada: matriz
  */
void printMatriz(Matriz * matriz) {
	int i, j;
	for (i = 0; i < matriz->x; i++) {
		for (j = 0; j < matriz->y; j++) {
			//printf("[R=%d G=%d B=%d] ", matriz->matriz[i][j].r, matriz->matriz[i][j].g, matriz->matriz[i][j].b);
		}
		//printf("\n");
	}
}

/*Funcion que muestra un arrelgo de pixeles por pantalla
  Entrada: arreglo de pixeles, cantidad de pixeles
  */
void printArregloPixeles(IMAGE * pixeles, int nPixeles) {
	int i;
	for (i = 0; i < nPixeles; i++) {
		//printf("%d.- R = %d G = %d B = %d\n", i, pixeles[i].r, pixeles[i].g, pixeles[i].b);
	}
}

/*Funcion que reduce las filas pares
  Entrada: matriz a reducir, matriz donde se alamacena resultado, cantidad de pixeles
  */
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

/*Funcion que reduce las filas impares
  Entrada: matriz a reducir, matriz donde se alamacena resultado, cantidad de pixeles
  */
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

/*Funcion que inserta pixeles en una columna
  Entrada: Pixeles a insertar, cantidad de pixeles, tamaño de la matriz, puntero a matrizFinal
  */ 
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

/*Fucion que reduce por columnas 
  Entradas: matriz a reducir, matriz donde insertar cambios, cantidad de pixeles
  */
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

/*NO SE USA: Funcion que obtiene la imagen desde un fichero
  Entrada: nombre del fichero
  */
IMGINFO* obtenerImagen(char* nombre)
{
	IMGINFO* imginfo=(IMGINFO*)malloc(sizeof(IMGINFO));
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
	printf("%d+%d", sizeof(FILEHEADER), sizeof(INFOHEADER));

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
	unsigned int pixelBytesPerRow = ih.width * 3;
	unsigned int paddingBytesPerRow = (4 - (pixelBytesPerRow % 4)) % 4;
	fseek(img, fh.imageDataOffset, SEEK_SET);
	IMAGE aux;
	//printf("asdsadn\n");

	int cont = 0;
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
	//printf("redi %d\n", cont);

	//printf("puntero asignado\n");
	fclose(img);
	//printf("fichero cerrado\n");
	imginfo->fh=fh;
	imginfo->ih=ih;
	imginfo->img=im;
	//fclose(s);
	return imginfo;
}

/*NO SE USA: Funcion que retorna la matriz de pixeles desde un fichero
  Entrada: nombre del fichero
  */
IMGINFO* obtenerMatriz(char* nombre)
{
	IMGINFO* imgaux = obtenerImagen(nombre);
	Matriz* m = (Matriz*)malloc(sizeof(Matriz));
	if (m == NULL) {
		return NULL;
	}
	//printf("poner matriz\n");
	m->matriz = imgaux->img;
	//printf("matriz puesta\n");
	m->x = imgaux->ih.height;
	m->y = imgaux->ih.width;
	//printf("asdasd%dx%d\n",imgaux->ih.height,imgaux->ih.width);
	imgaux->m=m;
	return imgaux;
}

/*Funcion que reduce la matriz por filas
  Entrada: matriz original, matriz para guardar cambios, cantidad de pixeles
  */
void reducirPorFilas(Matriz * matriz, Matriz * matrizFinal, int nPixeles) {
	//printf("APLICANDO REDUCCION nPixeles = %d\n", nPixeles);
	reducirPorFilasPares(matriz, matrizFinal, nPixeles);
	reducirPorFilasImpares(matriz, matrizFinal, nPixeles);
	////printMatriz(matrizFinal);
	//printf("TERMINA REDUCCION\n");
}

/*Funcion que abre retorna estructura con la imagen recibiendo una ruta
  Entrada: punteron a estructura donde se guarda informacion, ruta del fichero
  */
void abrir_imagen(BMP *imagen, char *ruta)
{
	FILE *archivo;	//Puntero FILE para el archivo de imágen a abrir
	int i,j;
	
	//Abrir el archivo de imágen
	archivo = fopen( ruta, "rb+" );
	if(!archivo)
	{ 
		//Si la imágen no se encuentra en la ruta dada
		printf( "La imagen %s no se encontro\n",ruta);
		exit(1);
	}

	//Leer la cabecera de la imagen y almacenarla en la estructura a la que apunta imagen
	fseek( archivo,0, SEEK_SET);
	fread(&imagen->bm,sizeof(char),2, archivo);
	fread(&imagen->tamano,sizeof(int),1, archivo);
	fread(&imagen->reservado,sizeof(int),1, archivo);	
	fread(&imagen->offset,sizeof(int),1, archivo);	
	fread(&imagen->tamanoMetadatos,sizeof(int),1, archivo);	
		
	fread(&imagen->ancho,sizeof(int),1, archivo);
	fread(&imagen->alto,sizeof(int),1, archivo);	
	fread(&imagen->numeroPlanos,sizeof(short int),1, archivo);	
	fread(&imagen->profundidadColor,sizeof(short int),1, archivo);	
	fread(&imagen->tipoCompresion,sizeof(int),1, archivo);
	fread(&imagen->tamanoEstructura,sizeof(int),1, archivo);
	fread(&imagen->pxmh,sizeof(int),1, archivo);
	fread(&imagen->pxmv,sizeof(int),1, archivo);
	fread(&imagen->coloresUsados,sizeof(int),1, archivo);
	fread(&imagen->coloresImportantes,sizeof(int),1, archivo);	
	//printf("header leido\n");

	//Validar ciertos datos de la cabecera de la imágen	
	if (imagen->bm[0]!='B'||imagen->bm[1]!='M')	
	{
		printf ("La imagen debe ser un bitmap.\n"); 
		exit(1);
	}
	if (imagen->profundidadColor!= 24) 
	{
		printf ("La imagen debe ser de 24 bits.\n"); 
		exit(1);
	}

	//Reservar memoria para el arreglo que tendra la imágen en escala de grises (Arreglo de tamaño "img.ancho X img.alto")
	/*imagen->pixel=malloc (imagen->alto* sizeof(char *)); 
	for( i=0; i<imagen->alto; i++)
		imagen->pixel[i]=malloc (imagen->ancho* sizeof(char)); */
	imagen->pixel= (IMAGE**)malloc(imagen->alto*sizeof(IMAGE*));
    for ( i=0; i<imagen->alto; i++){
        imagen->pixel[i]=(IMAGE*)malloc(imagen->ancho*sizeof(IMAGE));
    }

	//Pasar la imágen a el arreglo reservado en escala de grises
	unsigned char R,B,G;
	for (i=0;i<imagen->alto;i++)
	{
		for (j=0;j<imagen->ancho;j++)
		{  
			fread(&B,sizeof(char),1, archivo);  //Byte Blue del pixel
			fread(&G,sizeof(char),1, archivo);  //Byte Green del pixel
			fread(&R,sizeof(char),1, archivo);  //Byte Red del pixel
			//Conversión a escala de grises
			imagen->pixel[i][j].b=(unsigned char)B;
            imagen->pixel[i][j].g=(unsigned char)G; 
            imagen->pixel[i][j].r=(unsigned char)R;  	//Formula correcta
			//imagen->pixel[i][j]=(B+G+R)/3;								//Forma simple (Promedio)
		}   
	}
	//printf("pixeles leidos\n");
	imagen->m=(Matriz*)malloc(sizeof(Matriz));
	imagen->m->matriz=imagen->pixel;
		//printf("%d x %d\n",imagen->alto,imagen->ancho);
	imagen->m->x=imagen->alto;
	imagen->m->y=imagen->ancho;
	//printf("matriz asignada\n");
	//Cerrrar el archivo
	fclose(archivo);	
}

/*Funcion que copia una matriz
  Entrada: matriz a copiar
  Salida: matriz copiada
  */
Matriz * copiarMatriz(Matriz * matriz){
	Matriz * matrizCopia = (Matriz*)malloc(sizeof(Matriz));
	int i,j;
	matrizCopia->matriz=(IMAGE**)malloc(sizeof(IMAGE*)*matriz->x);
	for(i=0;i<matriz->x;i++){
		matrizCopia->matriz[i]=(IMAGE*)malloc(sizeof(IMAGE)*matriz->y);
	}
	for(i=0;i<matriz->x;i++){
		for(j=0;j<matriz->y;j++){
			matrizCopia->matriz[i][j].r=matriz->matriz[i][j].r;
			matrizCopia->matriz[i][j].g=matriz->matriz[i][j].g;
			matrizCopia->matriz[i][j].b=matriz->matriz[i][j].b;
		}
	}
	matrizCopia->x=matriz->x;
	matrizCopia->y=matriz->y;
	return matrizCopia;
}

/* Funcion que reduce la imagen
   Entrada: cantidad de reducciones, metodo a usar, cantidad de pixeles, nombre fichero de entrada, archivo de salida, archivo de salida2, flag de salida*/
void reducir(int nReducciones, int metodo, int nPixeles, char * entrada, FILE * salida1, FILE * salida2, int flag) {
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
	BMP img;
	abrir_imagen(&img,entrada);
	Matriz * matriz = img.m;
	Matriz * matriz2 = copiarMatriz(matriz);
	//printf("%dx%d\n",matriz->x,matriz->y);
	//printf("obtuveMatriz\n");
	//guardarFichero(salida2, matriz);
	Matriz * matrizFinal,*matrizFinal2;

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
			//printf("%dx%d\n",matriz->x,matriz->y);
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
						//printf("Dimensiones matriz x = %d, y = %d \n     Dimensiones matriz final x = %d  y = %d\n",matriz->x,matriz->y,matrizFinal->x,matrizFinal->y);

			clock_t start_t, end_t;
			double total_t;

			start_t =clock();
			reducirPorFilas(matriz, matrizFinal, nPixeles);
			end_t=clock();
			total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
			if(flag){
				printf("Tiempo total: %f\n", total_t);
			}
			//printf("CLOCK: %ld",CLOCKS_PER_SEC);
			matriz = matrizFinal;
			z++;
		}
		//printf("fichero1\n");
		guardarFichero(salida1, matrizFinal, &img);
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
			clock_t start_t, end_t;
			double total_t;

			start_t =clock();
			reducirPorColumnas(matriz, matrizFinal, nPixeles);
			end_t=clock();
			total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
			if(flag){
				printf("Tiempo total: %f\n", total_t);
			}
			//printf("CLOCK: %ld",CLOCKS_PER_SEC);
			matriz = matrizFinal;
			z++;
		}
		//printf("guaradndooo\n");
		guardarFichero(salida2, matrizFinal,&img);
	}
	else if(metodo == 3){
		int z = 0;
		while (z < nReducciones) {
			int i;
			//printf("*******************MATRIZ A REDUCIR**********************************\n");
			//printMatriz(matriz);
			//printf("nReducciones actual= %d----------------------------------------------------------------------\n", z);
			double div = ceil((double)matriz->y / nPixeles);
			int xMF = matriz->x, yMF = (int)div;
			//printf("%dx%d\n",matriz->x,matriz->y);
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
						//printf("Dimensiones matriz x = %d, y = %d \n     Dimensiones matriz final x = %d  y = %d\n",matriz->x,matriz->y,matrizFinal->x,matrizFinal->y);
			clock_t start_t, end_t;
			double total_t;

			start_t =clock();
			reducirPorFilas(matriz, matrizFinal, nPixeles);
			end_t=clock();
			total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
			if(flag){
				printf("Tiempo total: %f\n", total_t);
			}
			//printf("CLOCK: %ld",CLOCKS_PER_SEC);

			matriz = matrizFinal;
			z++;
		}
		//printf("fichero1\n");
		guardarFichero(salida1, matrizFinal, &img);
		z=0;
		while (z < nReducciones) {
			int i;
			//printf("*******************MATRIZ A REDUCIR**********************************\n");
			//printMatriz(matriz);
			//printf("nReducciones actual= %d----------------------------------------------------------------------\n", z);
			double div = ceil((double)matriz2->x / nPixeles);
			int yMF = matriz2->y, xMF = (int)div;
			//printf("nPixeles = %d, Dimensiones matriz final xMF= %d yMF = %d \n", nPixeles, xMF, yMF);
			matrizFinal2 = (Matriz*)malloc(sizeof(Matriz));
			matrizFinal2->matriz = (IMAGE**)malloc(sizeof(IMAGE*)*xMF);
			for (i = 0; i < xMF; i++) {
				matrizFinal2->matriz[i] = (IMAGE*)malloc(sizeof(IMAGE) * yMF);
			}
			for (i = 0; i < xMF; i++) {
				for (j = 0; j < yMF; j++) {
					matrizFinal2->matriz[i][j].r = -1;
					matrizFinal2->matriz[i][j].g = -1;
					matrizFinal2->matriz[i][j].b = -1;
				}
			}
			matrizFinal2->x = xMF;
			matrizFinal2->y = yMF;
			clock_t start_t, end_t;
			double total_t;

			start_t =clock();
			reducirPorColumnas(matriz2, matrizFinal2, nPixeles);
			printMatriz(matrizFinal);
			end_t=clock();
			total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
			if(flag){
				printf("Tiempo total: %f\n", total_t);
			}
			//printf("CLOCK: %ld",CLOCKS_PER_SEC);
			matriz2 = matrizFinal2;
			z++;
		}
		//printf("guaradndooo\n");
		guardarFichero(salida2, matrizFinal2,&img);
	}
}

/*Funcion que verifica que las entradas sean validas
  Entrada: puntero, archivo salida1, archivo salida2, cantidad de reducciones, cantida de pixeles, metodo, flag de salida
  Salida: booleano*/
int verificarEntradas(IMGINFO * fileEntrada, FILE * fileSalida1, FILE * fileSalida2, int nReducciones, int nPixeles, int metodo, int flag) {
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
	else if (nPixeles <= 0) {
		printf("La cantidad de pixeles debe ser mayor o igual a 1\n");
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
	int flag=0;
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
			flag=1;
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
	//printf("reducirrr\n");
	reducir(nReducciones, metodo, nPixeles, archivoEntrada, fileSalida1, fileSalida2, flag);
}

FILEHEADER* iniciarFileHeader()
{
	FILEHEADER* fh = (FILEHEADER*)malloc(sizeof(FILEHEADER));
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
	INFOHEADER* ih = (INFOHEADER*)malloc(sizeof(INFOHEADER));
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
	unsigned int pixelBytesPerRow = m->y * 3;
	unsigned int paddingBytesPerRow = (4 - (pixelBytesPerRow % 4)) % 4;
	static unsigned char zeroes[3] = {0, 0, 0};

	for (i = m->x - 1; i > -1; i--)
	{
		for (j = 0; j < m->y; j++)
		{
			aux = m->matriz[i][j];
			fwrite(&aux, sizeof(unsigned char), sizeof(IMAGE), f);
		}
		fwrite(zeroes, sizeof(unsigned char), paddingBytesPerRow, f);
	}
}

int guardarFichero(FILE* archivo, Matriz* m, BMP* imagen)
{
	int i,j;
	//imginfo->ih.width = m->y;
	//static unsigned char header[54] = {66, 77, 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24};
	//imginfo->ih.height = m->x;
		fseek( archivo,0, SEEK_SET);
	fwrite(&imagen->bm,sizeof(char),2, archivo);
	fwrite(&imagen->tamano,sizeof(int),1, archivo);	
	fwrite(&imagen->reservado,sizeof(int),1, archivo);	
	fwrite(&imagen->offset,sizeof(int),1, archivo);	
	fwrite(&imagen->tamanoMetadatos,sizeof(int),1, archivo);	
	
	fwrite(&m->y,sizeof(int),1, archivo);
	fwrite(&m->x,sizeof(int),1, archivo);		
	fwrite(&imagen->numeroPlanos,sizeof(short int),1, archivo);	
	fwrite(&imagen->profundidadColor,sizeof(short int),1, archivo);	
	fwrite(&imagen->tipoCompresion,sizeof(int),1, archivo);
	fwrite(&imagen->tamanoEstructura,sizeof(int),1, archivo);
	fwrite(&imagen->pxmh,sizeof(int),1, archivo);
	fwrite(&imagen->pxmv,sizeof(int),1, archivo);
	fwrite(&imagen->coloresUsados,sizeof(int),1, archivo);
	fwrite(&imagen->coloresImportantes,sizeof(int),1, archivo);	
	int imageSize = m->y * m->x;
	int file_size = 54 + 4 * imageSize;
	//imginfo->fh.bfSize = file_size;
	//imginfo->ih.biSizeImage = imageSize;
	//printf("escreibir headers\n");
	//printf("headers escritos\n");
	//fwrite(header, sizeof(unsigned char), 54, f);
	//printf("escribir matriz\n");
	writeMatriz(m, archivo);
}

int main(int argc, char **argv)
{
	init(argc, argv);
}