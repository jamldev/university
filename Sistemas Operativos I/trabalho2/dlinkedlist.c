#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>

struct Node{ 
	int espaco;
	int nprocesso;
	int inicio;
	int atual;
	int timeBlocked;
	struct Node* next; 
	struct Node* prev; 
};

//Função que altera o node
void changeNode(struct Node* node, int new_espaco, int nprocesso, int new_inicio, int new_atual, int new_timeBlocked){
	node->nprocesso = nprocesso;
	node->espaco = new_espaco;
	node->inicio = new_inicio;
	node->atual = new_atual;
	node->timeBlocked = new_timeBlocked;
}

//Função que apaga o node
void deleteNode(struct Node** head_ref, struct Node* del){
    if (*head_ref == NULL || del == NULL) 
        return;
    if (*head_ref == del) 
        *head_ref = del->next;
    if (del->next != NULL) 
        del->next->prev = del->prev;
    if (del->prev != NULL) 
        del->prev->next = del->next;
    free(del); 
    return; 
} 

//Função que coloca um novo node no inicio
void push(struct Node** head_ref, int new_espaco, int new_nprocesso, int new_inicio, int new_atual, int new_timeBlocked){
	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node)); 

	new_node->espaco = new_espaco;
	new_node->nprocesso = new_nprocesso;
	new_node->inicio = new_inicio;
	new_node->atual = new_atual;
	new_node->timeBlocked = new_timeBlocked;

	new_node->next = (*head_ref); 
	new_node->prev = NULL;
	if ((*head_ref) != NULL) 
		(*head_ref)->prev = new_node; 
	(*head_ref) = new_node; 
} 

//Função que insere node depois de certo node
void insertAfter(struct Node* prev_node, int new_espaco, int new_nprocesso, int new_inicio, int new_atual, int new_timeBlocked){ 
	if (prev_node == NULL) { 
		printf("O node anterior não pode ser NULL"); 
		return; 
	} 

	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));

	new_node->espaco = new_espaco;
	new_node->nprocesso = new_nprocesso;
	new_node->inicio = new_inicio;
	new_node->atual = new_atual;
	new_node->timeBlocked = new_timeBlocked;

	new_node->next = prev_node->next;
	prev_node->next = new_node;
	new_node->prev = prev_node;

	if (new_node->next != NULL) 
		new_node->next->prev = new_node; 
} 

//Função que coloca um novo node no fim
void append(struct Node** head_ref, int new_espaco, int new_nprocesso, int new_inicio, int new_atual, int new_timeBlocked){
	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node)); 
	struct Node* last = *head_ref;

	new_node->espaco = new_espaco;
	new_node->nprocesso = new_nprocesso;
	new_node->inicio = new_inicio; 
	new_node->atual = new_atual;
	new_node->timeBlocked = new_timeBlocked;

	new_node->next = NULL;
	if (*head_ref == NULL) { 
		new_node->prev = NULL; 
		*head_ref = new_node; 
		return; 
	}
	while (last->next != NULL) 
		last = last->next;
	last->next = new_node; 
	new_node->prev = last; 
	return; 
}

//Função que coloca um novo processo da linkedlist tendo em conta o espaço e o número do processo
bool inserirProcesso(struct Node* node, int espacoUsado, int nprocesso){
	int lowestEncontrado = 0, iniciolowestEncontrado = 0;
	bool encontrado = false;
	struct Node* pointer = NULL;
	while (node != NULL){
		if (node->espaco >= espacoUsado && node->nprocesso == 0){
			if(encontrado==false){
				lowestEncontrado = node->espaco;
				iniciolowestEncontrado = node->inicio;
				pointer = node;
				encontrado=true;
			}else if(lowestEncontrado > node->espaco){
				lowestEncontrado = node->espaco;
				iniciolowestEncontrado = node->inicio;
				pointer = node;
			}
		}
		node = node->next;
	}

	if(encontrado == true){
		changeNode(pointer, espacoUsado, nprocesso, iniciolowestEncontrado, iniciolowestEncontrado + 10, 0);
		if(lowestEncontrado-espacoUsado!=0)
			insertAfter(pointer, lowestEncontrado-espacoUsado, 0, iniciolowestEncontrado + espacoUsado, iniciolowestEncontrado + espacoUsado + 10, 0);
	}else
		return false;
	return true;
}

//Função que retira um processo da linkedlist tendo em conta o número do processo
bool removerProcessoBest(struct Node* node, int nprocesso){
	struct Node* header = node;
	while(node != NULL){
		if (node->nprocesso == nprocesso)
			break;
		node = node->next;
	}
	if(node != NULL){
		if(node != header){
			if (node->prev != NULL && node->prev->nprocesso == 0){
				changeNode(node, node->espaco + node->prev->espaco, 0, node->prev->inicio, node->prev->atual, 0);
				deleteNode(&header, node->prev);
			}
		}else if(node->next != NULL && node->next->nprocesso == 0){
			changeNode(node, node->espaco + node->next->espaco, 0, node->inicio, node->inicio, 0);
			deleteNode(&header, node->next);
		}else
			changeNode(node, node->espaco, 0, node->inicio, node->inicio, 0);
	}else{
		printf("erro a remover P%d", nprocesso);
		return false;
	}
	return true;
}


//Função que imprime a linkedlist 
void printList(struct Node* node){
	printf("lista:\n"); 
	while (node != NULL) { 
		printf("%d %d %d %d %d\n", node->espaco, node->nprocesso, node->inicio, node->atual, node->timeBlocked);
		node = node->next; 
	}
}

//Função que retorna o espaço maximo disponivel da linkedlist
int returnMaxAvailable(struct Node* head){
	int max=0;
	while(head != NULL){
		if(max < head->espaco)
			max = head->espaco;
		head = head->next;
	}
	return max;
}

//Função que encontra e retorna um node a partir do processo
struct Node* findNode(struct Node* node, int processo){
	while(node != NULL){
        if (node->nprocesso == processo)
            break;
        node = node->next;
    }
    return node;
}