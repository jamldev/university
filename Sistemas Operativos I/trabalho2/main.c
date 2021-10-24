#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "queue.c"
#include "dlinkedlist.c"

#define MEMSIZE 200                 //Tamanho do array mem
#define PAGINA 10                   //Tamanho de cada Página  
#define PAGASSIST 20                //Tamanho do array auxiliar pagingAssist, cujo tamanho é MEMSIZE/PÁGINA -> 200/10
#define MAXPROCESSOS 20             //Tamanho da quantidade máxima de processos
#define QUANTUM 3                   //Valor do Quantum
#define SIZE 500                    //Tamanho da quantidade máxima de cada linha
#define NOMEFICHEIRO "teste1.txt"   //Nome do ficheiro a testar  (ex: "teste1.txt", "teste2.txt")
#define MODO "so_best"              //Nome do modo a utilizar, neste caso apenas existe Paginação "so_pag", e BestFit "so_best"

#include "IO.c"
#include "GlobalFunctions.c"
#include "MemoryBest.c"
#include "MemoryPaging.c"

//Inicialização do programa
int main(){
    int linhas = contalinhas(NOMEFICHEIRO);
    int inputsave[linhas][2];
    openfileteste(linhas, inputsave, NOMEFICHEIRO);
    if(strcmp(MODO, "so_best") == 0){
        executionBest(linhas, inputsave);
    }else if(strcmp(MODO, "so_pag") == 0){
        executionPaging(linhas, inputsave);
    }else
        printf("Modo inválido");
    return 0;
}