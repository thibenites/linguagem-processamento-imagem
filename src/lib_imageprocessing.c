#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <FreeImage.h>

#include "imageprocessing.h"

#define n_threads 1

typedef struct {
	int posicao;
	float valor;
	imagem * I;
} Argumentos;

/*
imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo);
void liberar_imagem(imagem *i);
 */

imagem abrir_imagem(char *nome_do_arquivo) {
  FIBITMAP *bitmapIn;
  int x, y;
  //double valor_max_red = 0, valor_max_green = 0, valor_max_blue = 0;
  RGBQUAD color;
  imagem I;

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
    printf("Erro! Nao achei arquivo - %s\n", nome_do_arquivo);
  } else {
    //printf("Arquivo lido corretamente!\n");
   }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  
  I.r = malloc(sizeof(float) * x * y);
  I.g = malloc(sizeof(float) * x * y);
  I.b = malloc(sizeof(float) * x * y);

   for (int i=0; i<x; i++) {
     for (int j=0; j<y; j++) {
      int idx;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      //if(I.r[idx] > valor_max_red){
      	//(valor_max_red = I.r[idx]);
      //}
      I.g[idx] = color.rgbGreen;
      //if(I.g[idx] > valor_max_green){
      	//(valor_max_green = I.g[idx]);
      //}
      I.b[idx] = color.rgbBlue;
      //if(I.b[idx] > valor_max_blue){
      	//(valor_max_blue = I.b[idx]);
      //}
    }
   }
   /*printf("Valor máximo do bit vermelho: %lf.\n", valor_max_red);
   printf("Valor máximo do bit verde: %lf.\n", valor_max_green);
   printf("Valor máximo do bit azul: %lf.\n", valor_max_blue);*/
   printf("Arquivo lido!\n");
  return I;

}

void salvar_imagem(char *nome_do_arquivo, imagem *I) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;

  //printf("Salvando imagem %d por %d...\n", I->width, I->height);
  bitmapOut = FreeImage_Allocate(I->width, I->height, 24, 0, 0, 0);

   for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);
      color.rgbRed = I->r[idx];
      color.rgbGreen = I->g[idx];
      color.rgbBlue = I->b[idx];

      FreeImage_SetPixelColor(bitmapOut, i, j, &color);
    }
  }

  FreeImage_Save(FIF_JPEG, bitmapOut, nome_do_arquivo, JPEG_DEFAULT);
  printf("Imagem salva!\n");
}

//Aplicacao de brilho original
void aplicar_brilho(imagem *I, float valor) {

  clock_t t;
  t = clock();

  for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);
      //printf("R:%f G:%f B:%f\n",I->r[idx],I->g[idx],I->b[idx]);
      I->r[idx] = I->r[idx] * valor;
      if(I->r[idx] > 255)
      	I->r[idx] = 255;
      I->g[idx] = I->g[idx] * valor;
      if(I->g[idx] > 255)
      	I->g[idx] = 255;
      I->b[idx] = I->b[idx] * valor;
      if(I->b[idx] > 255)
      	I->b[idx] = 255;
    }
  }

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

  printf("Brilho aplicado! Procedimento original realizado em %f segundos.\n", time_taken);
}

//Aplicacao de brilho com multithread
void* mult_thread(void *arg) {
  Argumentos* N = (Argumentos*)arg;
  int idx = N->posicao;
  float valor = N->valor;
  imagem *I = N->I;

  I->r[idx] = I->r[idx] * valor;
  if(I->r[idx] > 255)
  	I->r[idx] = 255;

  I->g[idx] = I->g[idx] * valor;
  if(I->g[idx] > 255)
  	I->g[idx] = 255;
  
  I->b[idx] = I->b[idx] * valor;
  if(I->b[idx] > 255)
  	I->b[idx] = 255;
  
  return NULL;
}

void aplicar_brilho_thr(imagem *I, float valor) {
  clock_t t;
  t = clock();

  pthread_t threads[n_threads];
  Argumentos thread_args[n_threads];

  //Varre a matriz por linhas
  for (int i = 0; i < I->height; i++) {
  	int N = (int)((float)(I->width)/n_threads + 0.5); //Quantidade de vetores de threads necessarios
  	//printf("N=%d, width=%d, n_threads=%d\n",N,I->width,n_threads);
  	//Varre os blocos de pixels de tamanho n_threads
  	for(int j = 0; j*(n_threads) < I->width; j++){
  		//Varre os pixels dentro de cada bloco
  		for(int k = 0; k < n_threads; k++){
  			//Verifica se os pixels da linha ainda estao dentro da imagem
  			if((j*n_threads) + k < I->width){
  				//printf("i=%d, j=%d, k=%d, posicao=%d\n",i,j,k,i*I->width + (j*n_threads) + k);
  				thread_args[k].posicao = i*I->width + (j*n_threads) + k;
  				thread_args[k].valor = valor;
  				thread_args[k].I = I;
  			}
  		}
  		pthread_create(&(threads[k]), NULL, mult_thread, &(thread_args[k]));
  		//Esperando threads terminarem!
     	for (int k = 0; k < n_threads; k++){
     		if((j*n_threads) + k < I->width)
    			pthread_join(threads[k], NULL);
 		 }
  	}
  }

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

  printf("Brilho aplicado! Procedimento com multithreads realizado em %f segundos.\n", time_taken);
}

//Aplicacao de brilho com multiprocessos
void aplicar_brilho_prc(imagem *I, float valor) {
  clock_t t;
  t = clock();

  

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

  printf("Brilho aplicado! Procedimento com multiprocessos realizado em %f segundos.\n", time_taken);
}

void valor_maximo(imagem *I){
  float maximo = 0;

  for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;
      float intensidade;

      idx = i + (j*I->width);
      //printf("R:%f G:%f B:%f\n",I->r[idx],I->g[idx],I->b[idx]);
      intensidade = (I->r[idx] + I->g[idx] + I->b[idx])/3.0;
      if(intensidade > maximo)
        maximo = intensidade;
    }
  }
  printf("Intensidade maxima: %.2f\n", maximo);
}
