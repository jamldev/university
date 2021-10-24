#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define hashSIZE 3000
#define inputSIZE 100
#define alunoSIZE 30000000
#define idSIZE 6
#define seekSize (sizeof(char)*16)
#define paisSIZE 2

//Struct de cada Pais
struct dataPais{
   int alunosTotal;
   int alunosAtivos;
   int alunosTerminou;
   int alunosAbandonou;
   char pais[paisSIZE];
};

void clearhash(int SIZE, struct dataPais hashArray[]);
int hashCodePais(int SIZE, char pais[]);
int hashPosPais(int hashIndex, int SIZE, struct dataPais hashArray[], char pais[]);
void inserirInfo(int SIZE, struct dataPais hashArray[], char pais[], int aluno, int alunoAtivo, int alunoTerminou, int alunoAbandonou);
void alterarInfo(int SIZE, struct dataPais hashArray[], char pais[], int aluno, int alunoAtivo, int alunoTerminou, int alunoAbandonou);
void dadosPais(int SIZE, struct dataPais hashArray[], char pais[]);
