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
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;
    }
   }
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

//AINDA NAO ESTA FUNCIONANDO
imagem aplicar_brilho(imagem *I, float valor) {
  imagem J;

  J.height = I->height;
  J.width = I->width;

  printf("height: %d\n",J.height);
  printf("width: %d\n",J.width);

  for (int i=0; i<J.width; i++) {
     for (int j=0; j<J.height; j++) {
      int idx;

      idx = i + (j*J.width);
      printf("R:%f G:%f B:%f\n",I->r[idx],I->g[idx],I->b[idx]);
      J.r[idx] = I->r[idx] * valor;
      J.g[idx] = I->g[idx] * valor;
      //NAO CONSIGO ACESSAR O VETOR DE BLUE DA IMAGEM J!!!
      //J.b[idx] = I->b[idx] * valor;
    }
  }

  printf("Brilho aplicado!\n");

  return J;
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