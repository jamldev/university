///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//INSTRUCOES - BEST
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


//Função que coloca o processo no array mem
void ColocarNoMemBest(int linhas, int inputsave[linhas][2], int mem[MEMSIZE], struct Node* node, int linhaAtual, int processoAtual){
    while(node != NULL){
        if (node->nprocesso == processoAtual)
            break;
        node = node->next;
    }
    for(int i=node->inicio; i<node->inicio+10; i++)
        mem[i]=0;
    for(int j=node->atual; j<node->inicio + node->espaco; j+=2){
        mem[j] = inputsave[linhaAtual][0];
        mem[j+1] = inputsave[linhaAtual][1];
        linhaAtual++;
    }
}

//Função que auxilia a instrução FRK, copia um processo e coloca um semelhante(em que certos valores são trocados) no array mem
void MemToMemBest(int mem[MEMSIZE], struct Node* node, struct Node* nodeFRK, int *processoAtual){
    int k = node->inicio;
    for(int i=nodeFRK->inicio; i<nodeFRK->inicio+10; i++){
        mem[i]=mem[k];
        k++;
    }
    k = node->inicio+10;
    for(int j=nodeFRK->inicio+10; j<(nodeFRK->inicio + nodeFRK->espaco); j++){
        mem[j] = mem[k];
        k++;
    }
    //Coloca no pai o PID do filho
    mem[node->inicio + mem[node->atual+1]]=*processoAtual;
    //Coloca no filho o valor 0
    mem[nodeFRK->inicio + mem[node->atual+1]]=0;
}

//Função que coloca um novo processo no programa se o instante do processo corresponder ao instante atual
void entrarProcessoBest(int linhas, int inputsave[linhas][2], int mem[MEMSIZE], struct Node* head, struct Queue* ready, int instante, int *INIAtual, int *processoAtual){
    bool loopinstantesiguais = true;
    if(*INIAtual<linhas){
        while(loopinstantesiguais){
            if(inputsave[*INIAtual][1]!=instante)
                loopinstantesiguais = false;
            else{
                int SaveINI = *INIAtual;
                NextINI(linhas, inputsave, INIAtual);
                int ProcessoSize = ((*INIAtual-1) - SaveINI)*2 + 10;
                if(inserirProcesso(head, ProcessoSize, *processoAtual)){
                    ColocarNoMemBest(linhas, inputsave, mem, head, SaveINI+1, *processoAtual);
                    enQueue(ready, *processoAtual);
                }else
                    printf("Processo %d falha por falta de espaço\n", *processoAtual);
                *processoAtual+=1;
            }
         }
    }
}

//Função que coloca um processo do estado Blocked para o estado Ready se o tempo do estado Blocked acabou
void blocked_readyBest(struct Queue* blocked, struct Queue* ready, struct Node* node){
    struct Node* head = node;
    if(!isEmpty(blocked)){
        for(int i=0; i<SizeQueue(blocked); i++){
            node = head;
            int processo = deQueue(blocked);
            while(node != NULL){
                if (node->nprocesso == processo)
                    break;
                node = node->next;
            }
            if(node->timeBlocked == 0){
                enQueue(ready, processo);
                i--;
            }else{
                node->timeBlocked--;
                enQueue(blocked, processo);
            }
        }
    }
}

//Função que executa a instrução ZER
void executeZERBest(struct Node* node, int mem[MEMSIZE]){
    mem[node->inicio + mem[node->atual+1]] = 0;
    node->atual+=2;
}

//Função que executa a instrução INC
void executeINCBest(struct Node* node, int mem[MEMSIZE]){
    mem[node->inicio + mem[node->atual+1]]++;
    node->atual+=2;
}

//Função que executa a instrução DEC
void executeDECBest(struct Node* node, int mem[MEMSIZE]){
    mem[node->inicio + mem[node->atual+1]]--;
    node->atual+=2;
}

//Função que executa a instrução FRK
void executeFRKBest(struct Node* head, struct Node* node, int mem[MEMSIZE], int *processoAtual, struct Queue* ready){
    struct Node* nodeFRK = head;
    if(inserirProcesso(head, node->espaco, *processoAtual)){
        while(nodeFRK != NULL){
            if (nodeFRK->nprocesso == *processoAtual)
                break;
            nodeFRK = nodeFRK->next;
        }
        node->atual+=2;
        nodeFRK->atual = nodeFRK->inicio + (node->atual - node->inicio);
        MemToMemBest(mem, node, nodeFRK, processoAtual);
        enQueue(ready, *processoAtual);
        *processoAtual+=1;
    }
}

//Função que executa a instrução JFW
void executeJFWBest(struct Node* node, int mem[MEMSIZE]){
    if(mem[node->atual+1] != 0)
        node->atual += mem[node->atual+1]*2;
    else
        node->atual+=2;
}

//Função que executa a instrução JBK
void executeJBKBest(struct Node* node, int mem[MEMSIZE]){
    if(mem[node->atual+1] != 0)
        node->atual -= mem[node->atual+1]*2;
    else
        node->atual+=2;
}

//Função que executa a instrução DSK
void executeDSKBest(struct Node* node, int mem[MEMSIZE], struct Queue* run, struct Queue* blocked){
    node->timeBlocked = 5;
    enQueue(blocked, deQueue(run));
    node->atual+=2;
}

//Função que executa a instrução JIZ
void executeJIZBest(struct Node* node, int mem[MEMSIZE]){
    if(mem[node->inicio + mem[node->atual+1]] == 0){
        node->atual+=4;
    }else
        node->atual+=2;
}

//Função que executa a instrução PRT
void executePRTBest(struct Node* node, int mem[MEMSIZE], int *standOutput, bool *standOutputValid){
    *standOutput = mem[node->inicio + mem[node->atual+1]];
    *standOutputValid = true;
    node->atual+=2;
}

//Função que executa a instrução HLT
void executeHLTBest(struct Node* node, struct Queue* run){
    removerProcessoBest(node, deQueue(run));
}

//Função que verifica se a instrução FRK é possível
bool checkFRKBest(int mem[MEMSIZE], struct Node* head, struct Node* node, int processoAtual){
    if(returnMaxAvailable(head) > node->espaco)
        return false;
    else{
    	node->atual+=2;
        printf("fork sem sucesso\n");
    }
    return true;
}

//Função que verifica se a instrução JFW é possível
bool checkJumpLimitFowardBest(int mem[MEMSIZE], struct Node* node){
    if (node->inicio + node->espaco < node->atual + mem[node->atual+1]*2){
        printf("erro de segmentação do processo %d\n", node->nprocesso);
        return true;
    }
    return false;
}

//Função que verifica se a instrução JIZ é possível
bool checkJizFowardBest(int mem[MEMSIZE], struct Node* node){
    int jump;
    if(mem[node->inicio + mem[node->atual+1]] == 0)
        jump=2;
    else
        jump=1;
    if (node->inicio + node->espaco < node->atual + jump*2){
        printf("erro de segmentação do processo %d\n", node->nprocesso);
        return true;
    }
    return false;
}

//Função que verifica se a instrução JBK é possível
bool checkJumpLimitBackBest(int mem[MEMSIZE], struct Node* node){
    if (node->atual - mem[node->atual+1]*2 < node->inicio+10){
        printf("erro de segmentação do processo %d\n", node->nprocesso);
        return true;
    }
    return false;
}

//Função que verifica se a variável que a instrução utiliza é possível
bool checkVarAccessBest(int mem[MEMSIZE], struct Node* node) {
    if (mem[node->atual+1] > 9 || mem[node->atual+1] < 0){
        printf("erro de segmentação do processo %d\n", node->nprocesso);
        return true;
    }
    return false;           
}

//Função que verifica se a instrução pertence ao processo
bool checkLimitRangeBest(int mem[MEMSIZE], struct Node* node){
    if (node->atual > node->inicio+node->espaco-1){
        printf("erro de segmentação do processo %d\n", node->nprocesso);
        return true;
    }
    return false;
}

//Função que verifica todas as exceções da execução do programa
bool checkExceptionBest(int mem[MEMSIZE], struct Queue* run, struct Node* node1, int processoAtual, bool *checkFRK){
    struct Node* head = node1;
    struct Node* node = findNode(node1, inicio(run));
    if(checkLimitRangeBest(mem, node))
        return true;
    switch(mem[node->atual]){
        case 0:
            if(checkVarAccessBest(mem, node))
                return true;
            break;
        case 1:
            if(checkVarAccessBest(mem, node))
                return true;
            break;
        case 2:
            if(checkVarAccessBest(mem, node))
                return true;
            break;
        case 3:
            if(checkFRKBest(mem, head, node, processoAtual)){
            	*checkFRK = true;
                return true;
            }
            break;
        case 4:
            if(checkJumpLimitFowardBest(mem, node))
                return true;
            break;
        case 5: 
            if(checkJumpLimitBackBest(mem, node))
                return true;
            break;
        case 7:
            if(checkJizFowardBest(mem, node))
                return true;
            break;
        case 8:
            if(checkVarAccessBest(mem, node))
                return true;
            break;
        default:
            return false;
            break;
    }
    return false;
}

//Função que executa as instruções antes da impressão do programa
bool executeRunBeforeBest(int mem[MEMSIZE], struct Queue* ready, struct Queue* run, struct Queue* blocked, struct Node* node1, int *processoAtual, int *standOutput, bool *standOutputValid){
    struct Node* head = node1;
    struct Node* node = findNode(node1, inicio(run));
    switch(mem[node->atual]){
        case 0:
            executeZERBest(node, mem);
            return true;
        case 1:
            executeINCBest(node, mem);
            return true;
        case 2:
            executeDECBest(node, mem);
            return true;
        case 3:
            executeFRKBest(head, node, mem, processoAtual, ready);
            return true;
        case 4:
            executeJFWBest(node, mem);
            return true;
        case 5:
            executeJBKBest(node, mem);
            return true;
        case 7:
            executeJIZBest(node, mem);
            return true;
        case 8:
            executePRTBest(node, mem, standOutput, standOutputValid);
            return true;
        default:
        	return false;
    }
}

//Função que executa as instruções depois da impressão do programa
void executeRunAfterBest(int mem[MEMSIZE], struct Queue* ready, struct Queue* run, struct Queue* blocked, struct Node* node1, int *processoAtual){
    struct Node* node = findNode(node1, inicio(run));
    switch(mem[node->atual]){
        case 6:
            executeDSKBest(node, mem, run, blocked);
            break;
        case 9:
            executeHLTBest(node, run);
            break;
    }
}

//Função que executa o programa a partir do algoritmo BestFit
void executionBest(int linhas, int inputsave[linhas][2]) {
    int instante=0, INIAtual=0, processoAtual=1, standOutput=-1, quantumTimer = QUANTUM;
    int mem[MEMSIZE];

    struct Node* head = NULL;
    struct Node* node;
    append(&head, MEMSIZE, 0, 0, 0, 0);

    struct Queue* ready = createQueue(linhas);
    struct Queue* run = createQueue(1);
    struct Queue* blocked = createQueue(linhas);

    while(SizeQueue(ready)>0 || SizeQueue(run)>0 || SizeQueue(blocked)>0 || INIAtual<linhas){
        bool standOutputValid = false, checkFRK = false;
        entrarProcessoBest(linhas, inputsave, mem, head, ready, instante, &INIAtual, &processoAtual);
        if(quantumTimer <= 0){
            enQueue(ready, deQueue(run));
            quantumTimer = QUANTUM;
        }
        ready_run(ready, run);
        if(isEmpty(run)){
            blocked_readyBest(blocked, ready, head);
            ready_run(ready, run);
            if(!isEmpty(run)){
                if(!checkExceptionBest(mem, run, head, processoAtual, &checkFRK)){
                    if(executeRunBeforeBest(mem, ready, run, blocked, head, &processoAtual, &standOutput, &standOutputValid)){
                    	prints(instante, ready, run, blocked, standOutput, standOutputValid);
                    }else{
                    	prints(instante, ready, run, blocked, standOutput, standOutputValid);
                    	executeRunAfterBest(mem, ready, run, blocked, head, &processoAtual);
                    }
                }else{
                	if(!checkFRK){
                		node = findNode(head, inicio(run));
                    	removerProcessoBest(node, deQueue(run));
                	}
                    instante--;
                }
            }else
                prints(instante, ready, run, blocked, standOutput, standOutputValid);
        }else{
            if(!checkExceptionBest(mem, run, head, processoAtual, &checkFRK)){
                blocked_readyBest(blocked, ready, head);
                if(executeRunBeforeBest(mem, ready, run, blocked, head, &processoAtual, &standOutput, &standOutputValid)){
                    prints(instante, ready, run, blocked, standOutput, standOutputValid);
                }else{
                    prints(instante, ready, run, blocked, standOutput, standOutputValid);
                    executeRunAfterBest(mem, ready, run, blocked, head, &processoAtual);
                }   
            }else{
            	if(!checkFRK){
                	node = findNode(head, inicio(run));
                    removerProcessoBest(node, deQueue(run));
                }
                instante--;
            }
        }
        if(isEmpty(run))
            quantumTimer = QUANTUM;
        else
            quantumTimer--;
        instante++;
    }
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////