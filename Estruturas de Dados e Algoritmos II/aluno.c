#include "aluno.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


//Função que calcula o "hashCode" do aluno dependente do id
unsigned int hashCodeAluno(char id[]){
	unsigned int hash=0, f=11;
	for(short i=0; i<idSIZE; i++)
		hash = f*hash +(int)id[i];
	return hash % alunoSIZE;
}


//Função que verifica o "hashCode" do aluno e retorna o "hashCode" dependente do ficheiro
int hashPosAluno(unsigned int pos, char id[], FILE *f){
	bool loop = true;
	while(loop){
		char aux[idSIZE];
		fseek(f, seekSize*pos, SEEK_SET);
		if(fread(&aux, sizeof(aux), 1, f)){
			if(strncmp(aux, id, idSIZE)==0 || strcmp(aux, "")==0)
				return pos;
			pos++;
			if(pos>=alunoSIZE)
				pos=0;
		}else{
			return pos;
		}
	}
	return -1;
}


//Função que coloca o aluno no ficheiro
void aluno_to_file (int pos, struct dataAluno aluno, FILE *f) {
	fseek(f, seekSize*pos, SEEK_SET);
	fwrite(&aluno, sizeof(aluno), 1, f);
}


//Função que coloca a informação do ficheiro no aluno
struct dataAluno file_to_aluno(int pos, FILE *f) {
	struct dataAluno aluno;
	fseek(f, seekSize*pos, SEEK_SET);
	if(fread(&aluno, sizeof(aluno), 1, f))
		return aluno;
	return aluno;
}


//Função que coloca a informação do aluno sobre se terminou ou abandonou na variável info
void terminouAbandonouAluno(int pos, char id[], char *info, FILE *f){
	struct dataAluno aluno = file_to_aluno(pos, f);
	info[0]=aluno.alunoTerminou;
	info[1]=aluno.alunoAbandonou;
}


//Função booleana que retorna true se o aluno foi eliminado, e não se for o oposto
bool eliminado(int pos, char id[], FILE *f){
	struct dataAluno aluno = file_to_aluno(pos, f);
	if(aluno.eliminado == '0')
		return false;
	return true;
}



//Função booleana que retorna true se o aluno existe, e não se for o oposto
bool existeAluno(int pos, char id[], FILE *f){
	char aux[idSIZE];
	fseek(f, seekSize*pos, SEEK_SET);
	if(fread(&aux, sizeof(aux), 1, f)){
		if(strncmp(aux, id, idSIZE) == 0)
			return true;
		return false;
	}
	return false;
}

