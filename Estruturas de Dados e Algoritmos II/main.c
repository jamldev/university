#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hashtable.h"
#include "aluno.h"


//Função que passa a Hashtable para ficheiro
void hash_to_file(struct dataPais hashArray[], FILE *f){
    for(int i = 0; i < hashSIZE; i++){
        if(strcmp(hashArray[i].pais, "")!=0)
            fprintf(f,"%c;%c;%d;%d;%d;%d\n", hashArray[i].pais[0], hashArray[i].pais[1],hashArray[i].alunosTotal, hashArray[i].alunosAtivos, hashArray[i].alunosTerminou, hashArray[i].alunosAbandonou);
    }
}


//Função que passa a informação do ficheiro para a Hashtable
void file_to_hash(struct dataPais hashArray[], FILE *f){
	char line[inputSIZE], pais[paisSIZE], *p;
	while(fgets(line, sizeof(line), f) != NULL){
		int spot=0, hashIndex = -1;
		p = strtok(line, ";");
		while(p != NULL){
			switch(spot){
				case 0:
					pais[0] = p[0];
					break;
				case 1:
					pais[1] = p[0];
					hashIndex = hashPosPais(abs(hashCodePais(hashSIZE, pais)), hashSIZE, hashArray, pais);
					hashArray[hashIndex].pais[0] = pais[0];
					hashArray[hashIndex].pais[1] = pais[1];
					break;
				case 2:
					hashArray[hashIndex].alunosTotal = atoi(p);
					break;
				case 3:
					hashArray[hashIndex].alunosAtivos = atoi(p);
					break;
				case 4:
					hashArray[hashIndex].alunosTerminou = atoi(p);
					break;
				case 5:
					hashArray[hashIndex].alunosAbandonou = atoi(p);
					break;
			}
			p = strtok(NULL, ";");
			spot++;
		}
	}
	fclose(f);
}



//Função booleana que retorna true se o estudante foi introduzido, e retorna false se não foi
bool introduzirEstudante(char id[], char pais[], FILE *f){
	int pos = hashPosAluno(hashCodeAluno(id), id, f);
	if(!existeAluno(pos, id, f) || (existeAluno(pos, id, f) && eliminado(pos, id, f))){
		struct dataAluno aluno;
		memcpy(aluno.id, id, idSIZE);
		strncpy(aluno.pais, pais, paisSIZE);
		aluno.alunoTerminou = '0';
		aluno.alunoAbandonou = '0';
		aluno.eliminado = '0';
		//COLOCAR NO FICHEIRO:
		aluno_to_file(pos, aluno, f);
		return true;
	}else
		printf("+ estudante %c%c%c%c%c%c existe\n", id[0], id[1], id[2], id[3], id[4], id[5]);
	return false;
}



//Função booleana que retorna true se o id do estudante foi removido, e retorna false se não foi
bool removerId(char id[], char *pais, FILE *f){
	int pos = hashPosAluno(hashCodeAluno(id), id, f);
	if(existeAluno(pos, id, f) && !eliminado(pos, id, f)){
		char info[2];
		terminouAbandonouAluno(pos, id, info, f);
		if(info[0] == '1'){
			printf("+ estudante %c%c%c%c%c%c terminou\n", id[0], id[1], id[2], id[3], id[4], id[5]);
		}else if(info[1] == '1'){
			printf("+ estudante %c%c%c%c%c%c abandonou\n", id[0], id[1], id[2], id[3], id[4], id[5]);
		}else{
			struct dataAluno aluno;
			aluno = file_to_aluno(pos, f);
			aluno.eliminado = '1';
			pais[0] = aluno.pais[0];
			pais[1] = aluno.pais[1];
			//ALTERAR NO FICHEIRO:
			aluno_to_file(pos, aluno, f);
			return true;
		}
	}else
		printf("+ estudante %c%c%c%c%c%c inexistente\n", id[0], id[1], id[2], id[3], id[4], id[5]);
	return false;
}


//Função booleana que retorna true se o id do estudante foi removido, e retorna false se não foi
bool alunoTerminou(char id[], char *pais, FILE *f){
	int pos = hashPosAluno(hashCodeAluno(id), id, f); 
	if(existeAluno(pos, id, f) && !eliminado(pos, id, f)){
		char info[2];
		terminouAbandonouAluno(pos, id, info, f);
		if(info[0] == '1'){
			printf("+ estudante %c%c%c%c%c%c terminou\n", id[0], id[1], id[2], id[3], id[4], id[5]);
		}else if(info[1] == '1'){
			printf("+ estudante %c%c%c%c%c%c abandonou\n", id[0], id[1], id[2], id[3], id[4], id[5]);
		}else{
			struct dataAluno aluno;
			aluno = file_to_aluno(pos, f);
			aluno.alunoTerminou = '1';
			pais[0] = aluno.pais[0];
			pais[1] = aluno.pais[1];
			//ALTERAR NO FICHEIRO:
			aluno_to_file(pos, aluno, f);
			return true;
		}
	}else
		printf("+ estudante %c%c%c%c%c%c inexistente\n", id[0], id[1], id[2], id[3], id[4], id[5]);
	return false;
}


//Função booleana que retorna true se o id do estudante foi removido, e retorna false se não foi
bool alunoAbandonou(char id[], char *pais, FILE *f){
	int pos = hashPosAluno(hashCodeAluno(id), id, f); 
	if(existeAluno(pos, id, f) && !eliminado(pos, id, f)){
		char info[2];
		terminouAbandonouAluno(pos, id, info, f);
		if(info[0] == '1'){
			printf("+ estudante %c%c%c%c%c%c terminou\n", id[0], id[1], id[2], id[3], id[4], id[5]);
		}else if(info[1] == '1'){
			printf("+ estudante %c%c%c%c%c%c abandonou\n", id[0], id[1], id[2], id[3], id[4], id[5]);
		}else{
			struct dataAluno aluno;
			aluno = file_to_aluno(pos, f);
			aluno.alunoAbandonou = '1';
			pais[0] = aluno.pais[0];
			pais[1] = aluno.pais[1];
			//ALTERAR NO FICHEIRO:
			aluno_to_file(pos, aluno, f);
			return true;
		}
	}else
		printf("+ estudante %c%c%c%c%c%c inexistente\n", id[0], id[1], id[2], id[3], id[4], id[5]);
	return false;
}








int main() {

	///////////////////////////////////////////////////
	//   Carregar / Criar   Hashtable:
	///////////////////////////////////////////////////
	
	struct dataPais infoPais[hashSIZE];

	FILE *backup_hashtable;
	backup_hashtable = fopen("backup_hashtable.txt","rb+");
	
	clearhash(hashSIZE, infoPais);
	if(backup_hashtable != NULL)
    	file_to_hash(infoPais, backup_hashtable);
    

    ///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////


    ///////////////////////////////////////////////////
	//    File dos estudantes
	///////////////////////////////////////////////////
    

    FILE *backup_dataAluno;
	backup_dataAluno = fopen("backup_dataAluno.txt","rb+");
	if(backup_dataAluno == NULL)
    	backup_dataAluno = fopen("backup_dataAluno.txt","wb+");




    ///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////



	char input[inputSIZE], id[idSIZE], pais[paisSIZE];
	while (fgets(input, inputSIZE, stdin)){
		switch(input[0]){
			case 'I':
				memcpy(id, input+2, 6);
				strncpy(pais, input+9, 2);
				if(introduzirEstudante(id, pais, backup_dataAluno))
					inserirInfo(hashSIZE, infoPais, pais, 1, 1, 0, 0);
				break;
			case 'R':
				memcpy(id, input+2, 6);
				if(removerId(id, pais, backup_dataAluno))
					alterarInfo(hashSIZE, infoPais, pais, -1, -1, 0, 0);
				break;
			case 'T':
				memcpy(id, input+2, 6);
				strncpy(pais, input+9, 2);
				if(alunoTerminou(id, pais, backup_dataAluno))
					alterarInfo(hashSIZE, infoPais, pais, 0, -1, 1, 0);
				break;
			case 'A':
				memcpy(id, input+2, 6);
				strncpy(pais, input+9, 2);
				if(alunoAbandonou(id, pais, backup_dataAluno))
					alterarInfo(hashSIZE, infoPais, pais, 0, -1, 0, 1);
				break;
			case 'P':
				strncpy(pais, input+2, 2);
				dadosPais(hashSIZE, infoPais, pais);
				break;
			case 'X':
				backup_hashtable = fopen("backup_hashtable.txt","wb+");
				hash_to_file(infoPais, backup_hashtable);
				fclose(backup_hashtable);
				fclose(backup_dataAluno);
				return 0;
			default:
				printf("Invalid input\n");
				break;
		}
	}

	return 0;
	
}
