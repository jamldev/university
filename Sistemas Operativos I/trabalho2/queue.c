#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>

#define INTMIN (-2147483648)

struct Queue{ 
	int inicio, fim, quantidade; 
	unsigned capacidade; 
	int* array; 
};

//Cria uma Queue cujo tamanho máximo é recebida
struct Queue* createQueue(unsigned capacidade){ 
	struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue)); 
	queue->capacidade = capacidade; 
	queue->inicio = queue->quantidade = 0; 
	queue->fim = capacidade - 1;
	queue->array = (int*) malloc(queue->capacidade * sizeof(int)); 
	return queue;
} 

//Função booleana que retorna true se a Queue estiver cheia
bool isFull(struct Queue* queue){
	return (queue->quantidade == queue->capacidade);
}

//Função booleana que retorna true se a Queue estiver vazia
bool isEmpty(struct Queue* queue){
	return (queue->quantidade == 0);
} 

//Função que retorna a quantidade de elementos da Queue
int SizeQueue(struct Queue* queue){
	if (isEmpty(queue))
		return 0;
	return (queue->quantidade);
}

//Função que coloca o item na Queue
void enQueue(struct Queue* queue, int item){
	if (isFull(queue)) 
		return; 
	queue->fim = (queue->fim + 1)%queue->capacidade; 
	queue->array[queue->fim] = item; 
	queue->quantidade = queue->quantidade + 1;
} 

//Função que retira o item na Queue e retorna esse mesmo
int deQueue(struct Queue* queue){ 
	if (!isEmpty(queue)){
		int item = queue->array[queue->inicio]; 
		queue->inicio = (queue->inicio + 1)%queue->capacidade; 
		queue->quantidade = queue->quantidade - 1; 
		return item;
	}
	return INTMIN;
} 

//Função que retorna o inicio da Queue
int inicio(struct Queue* queue){ 
	if (!isEmpty(queue))
		return queue->array[queue->inicio];
	return INTMIN;
} 

//Função que retorna o fim da Queue
int fim(struct Queue* queue){ 
	if (!isEmpty(queue))  
		return queue->array[queue->fim];
	return INTMIN;
}