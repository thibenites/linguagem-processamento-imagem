%{
#include <stdio.h>
#include "imageprocessing.h"
#include <FreeImage.h>

void yyerror(char *c);
int yylex(void);

%}
%union {
  char    strval[50];
  int     ival;
  float   fval;
}
%token <strval> STRING
%token <ival> VAR IGUAL EOL ASPA VEZES DIVIDIDO ABRE FECHA
%token <fval> FLOAT
%left SOMA

%%

PROGRAMA:
        PROGRAMA EXPRESSAO EOL
        |
        ;

EXPRESSAO:
    | STRING IGUAL STRING {
        printf("Copiando %s para %s\n", $3, $1);
        imagem I = abrir_imagem($3);
        printf("Li imagem %d por %d\n", I.width, I.height);
        salvar_imagem($1, &I);
                          }
    ;

    | STRING IGUAL STRING VEZES FLOAT {
        printf("Aplicando brilho *%f\n", $5);
        imagem I = abrir_imagem($3);
        aplicar_brilho(&I,$5);
        salvar_imagem($1,&I);
                                       }
    ;

    | STRING IGUAL STRING DIVIDIDO FLOAT {
        printf("Aplicando brilho /%f\n", $5);
        imagem I = abrir_imagem($3);
        aplicar_brilho(&I,1/$5);
        salvar_imagem($1,&I);
                                       }
    ;
    | ABRE STRING FECHA {
        printf("Calcula maximo\n");
        imagem I = abrir_imagem($2);
        valor_maximo(&I);
                        }
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main() {
  FreeImage_Initialise(0);
  yyparse();
  return 0;
}