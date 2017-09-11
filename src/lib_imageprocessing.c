#include <stdlib.h>
#include <stdio.h>

#include "imageprocessing.h"

#include <FreeImage.h>

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
    printf("Arquivo lido corretamente!\n");
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
   printf("Acabou leitura!\n");
  return I;

}

void salvar_imagem(char *nome_do_arquivo, imagem *I) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;

  printf("Salvando imagem %d por %d...\n", I->width, I->height);
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

void aplicar_brilho(imagem *I, float valor) {

  for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);
      //printf("R:%f G:%f B:%f\n",I->r[idx],I->g[idx],I->b[idx]);
      I->r[idx] = I->r[idx] * valor;
      if(I->r[idx] > 255){
      	I->r[idx] = 255;
      }
      I->g[idx] = I->g[idx] * valor;
      if(I->g[idx] > 255){
      	I->g[idx] = 255;
      }
      I->b[idx] = I->b[idx] * valor;
      if(I->b[idx] > 255){
      	I->b[idx] = 255;
      }
    }
  }

  printf("Brilho aplicado!\n");
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
  printf("Intensidade maxima: %f\n", maximo);
}
