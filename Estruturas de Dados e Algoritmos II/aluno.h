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

//Struct de cada Aluno
struct dataAluno {
	char id[idSIZE];
	char pais[paisSIZE];
	char alunoTerminou;
	char alunoAbandonou;
	char eliminado;
};


unsigned int hashCodeAluno(char id[]);
int hashPosAluno(unsigned int pos, char id[], FILE *f);
void aluno_to_file (int pos, struct dataAluno aluno, FILE *f);
struct dataAluno file_to_aluno(int pos, FILE *f);
void terminouAbandonouAluno(int pos, char id[], char *info, FILE *f);
bool eliminado(int pos, char id[], FILE *f);
bool existeAluno(int pos, char id[], FILE *f);
