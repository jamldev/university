#include "hashtable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


//Função que dá "clear" na hashtable
void clearhash(int SIZE, struct dataPais hashArray[]){
	for(int i=0; i<SIZE; i++){
		strcpy(hashArray[i].pais, "");
      hashArray[i].alunosTotal = 0;
      hashArray[i].alunosAtivos = 0;
      hashArray[i].alunosTerminou = 0;
      hashArray[i].alunosAbandonou = 0;
   }
}


//Função que calcula o hashCode do Pais
int hashCodePais(int SIZE, char pais[]){
   return ((int)pais[0])*((int)pais[1]) % SIZE;
}


//Função que verifica o hashCode do Pais e retorna o hashCode "correto"
int hashPosPais(int hashIndex, int SIZE, struct dataPais hashArray[], char pais[]){
   int count=0;
   while(count<SIZE){
      if(strcmp(hashArray[hashIndex].pais, "")==0){
         break;
      }else if(strncmp(hashArray[hashIndex].pais, pais, paisSIZE)==0)
         break;
      hashIndex++;
      hashIndex %= SIZE;
      count++;
   }
   if(count>SIZE-1)
      return-1;
   return hashIndex;
}

//Função que insere informação nova na hashtable dos paises
void inserirInfo(int SIZE, struct dataPais hashArray[], char pais[], int aluno, int alunoAtivo, int alunoTerminou, int alunoAbandonou) {
   int hashIndex = hashPosPais(abs(hashCodePais(SIZE, pais)), SIZE, hashArray, pais);

   if(strcmp(hashArray[hashIndex].pais, "")==0){
   	strncpy(hashArray[hashIndex].pais, pais, paisSIZE);
      hashArray[hashIndex].alunosTotal = aluno;
      hashArray[hashIndex].alunosAtivos = alunoAtivo;
      hashArray[hashIndex].alunosTerminou = alunoTerminou;
      hashArray[hashIndex].alunosAbandonou = alunoAbandonou;
   }else{
      hashArray[hashIndex].alunosTotal += aluno;
      hashArray[hashIndex].alunosAtivos += alunoAtivo;
      hashArray[hashIndex].alunosTerminou += alunoTerminou;
      hashArray[hashIndex].alunosAbandonou += alunoAbandonou;
   }
}


//Função que altera informação na hashtable dos paises
void alterarInfo(int SIZE, struct dataPais hashArray[], char pais[], int aluno, int alunoAtivo, int alunoTerminou, int alunoAbandonou) {
   int hashIndex = hashPosPais(abs(hashCodePais(SIZE, pais)), SIZE, hashArray, pais);

   hashArray[hashIndex].alunosTotal += aluno;
   hashArray[hashIndex].alunosAtivos += alunoAtivo;
   hashArray[hashIndex].alunosTerminou += alunoTerminou;
   hashArray[hashIndex].alunosAbandonou += alunoAbandonou;
}


//Função que retorna a informação de certo pais da hashtable
void dadosPais(int SIZE, struct dataPais hashArray[], char pais[]){
	int hashIndex = hashPosPais(abs(hashCodePais(SIZE, pais)), SIZE, hashArray, pais);

	if(hashArray[hashIndex].alunosTotal <= 0){
		printf("+ sem dados sobre %c%c\n", pais[0], pais[1]);
	}else{
		printf("+ %c%c - correntes: %d, diplomados: %d, abandonaram: %d, total: %d\n", pais[0], pais[1], hashArray[hashIndex].alunosAtivos, hashArray[hashIndex].alunosTerminou, hashArray[hashIndex].alunosAbandonou, hashArray[hashIndex].alunosTotal);
	}
}
