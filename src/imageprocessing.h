#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

typedef struct{
  unsigned int width, height;
  float *r, *g, *b;
} imagem;

imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo, imagem *I);
void liberar_imagem(imagem *i);
void aplicar_brilho(imagem *I, float valor);
void aplicar_brilho_thr(imagem *I, float valor);
void aplicar_brilho_prc(imagem *I, float valor);
void valor_maximo(imagem *I);

#endif