///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//INSTRUCOES - PAGING
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

struct pagingElements{
    int processo;
    int nextPage;
    int backPage;
    int lineToRead;
    bool terminou;
    bool variaveis;
};

//Função que passa para a próxima linha do processo atual
void nextLinePaging(struct pagingElements pagingAssist[PAGASSIST], int paginaAtual){
	pagingAssist[paginaAtual].lineToRead+=2;
    if(pagingAssist[paginaAtual].lineToRead>=PAGINA)
        pagingAssist[paginaAtual].terminou = true;
}

//Função que inicializa o pagingAssist
void inicializePaging(struct pagingElements pagingAssist[PAGASSIST]){
    for(int i=0; i<PAGASSIST; i++){
        pagingAssist[i].processo = -1;
        pagingAssist[i].nextPage = -1;
        pagingAssist[i].backPage = -1;
        pagingAssist[i].lineToRead = 0;
        pagingAssist[i].terminou = false;
        pagingAssist[i].variaveis = false;
    }
}

//Função que retorna o espaço livre do pagingAssist;
int countSpacePaging(struct pagingElements pagingAssist[PAGASSIST]){
    int count=0;
    for(int i=0; i<PAGASSIST; i++){
        if(pagingAssist[i].processo == -1)
            count++;
    }
    return count;
}

//Função que retorna a página das variáveis do processo
int variablesPaging(struct pagingElements pagingAssist[PAGASSIST], int paginaAtual){
    int processo = pagingAssist[paginaAtual].processo, i;
    for(i=0; i<PAGASSIST; i++){
        if(pagingAssist[i].processo == processo && pagingAssist[i].variaveis)
            break;
    }
    return i;
}

//Função que insere um novo processo no array mem
void insertProcessPaging(int linhas, int inputsave[linhas][2], int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int linhaAtual, int processoAtual, int ProcessoSize){
    int linhasRestantes = ProcessoSize, i=0, iAnterior=-1;
    bool variaveisIniciais = true, acabou = false;
    while(linhasRestantes>0){
        while(pagingAssist[i].processo != -1)
            i++;
        for(int j = i*PAGINA; j < (i*PAGINA)+PAGINA; j+=2){
            if(variaveisIniciais){
                mem[j] = 0;
                mem[j+1] = 0;
            }else if(linhasRestantes>=PAGINA || j<(i*PAGINA)+linhasRestantes){
                mem[j] = inputsave[linhaAtual][0];
                mem[j+1] = inputsave[linhaAtual][1];
                linhaAtual++;
            }else{
                mem[j] = -1;
                mem[j+1] = -1;
            } 
        }
        linhasRestantes-=PAGINA;
        if(linhasRestantes<=0)
            acabou = true;
        pagingAssist[i].processo = processoAtual;
        pagingAssist[i].lineToRead = 0;
        pagingAssist[i].terminou = false;
        if(iAnterior != -1)
            pagingAssist[iAnterior].nextPage = i;
        if(acabou){
            pagingAssist[i].nextPage = -1;
            pagingAssist[i].backPage = iAnterior;
        }else
            pagingAssist[i].backPage = iAnterior;
        if(variaveisIniciais)
            pagingAssist[i].backPage = -1;
        pagingAssist[i].variaveis = variaveisIniciais;
        //Atualiza as variáveis iAnterior e variaveisIniciais
        iAnterior = i;
        variaveisIniciais = false;
    }
}

//Função que coloca um novo processo no programa se o instante do processo corresponder ao instante atual
void entrarProcessoPaging(int linhas, int inputsave[linhas][2], int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], struct Queue* ready, int instante, int *INIAtual, int *processoAtual){
    bool loopinstantesiguais = true;
    if(*INIAtual<linhas){
        while(loopinstantesiguais){
            if(inputsave[*INIAtual][1]!=instante)
                loopinstantesiguais = false;
            else{
                int SaveINI = *INIAtual;
                NextINI(linhas, inputsave, INIAtual);
                int ProcessoSize = ((*INIAtual-1) - SaveINI)*2 + 10;
                if(countSpacePaging(pagingAssist) >= ProcessoSize/10){
                    insertProcessPaging(linhas, inputsave, mem, pagingAssist, SaveINI+1, *processoAtual, ProcessoSize);
                    enQueue(ready, *processoAtual);
                }else
                    printf("Processo %d falha por falta de espaço\n", *processoAtual);
                *processoAtual+=1;
            }
         }
    }
}

//Função que coloca um processo do estado Blocked para o estado Ready se o tempo do estado Blocked acabou
void blocked_readyPaging(struct Queue* blocked, struct Queue* ready, int timeBlocked[MAXPROCESSOS]){
    if(!isEmpty(blocked)){
        for(int i=0; i<SizeQueue(blocked); i++){
            int processo = deQueue(blocked);
            if(timeBlocked[processo] == 0){
                enQueue(ready, processo);
                i--;
            }else{
                timeBlocked[processo]--;
                enQueue(blocked, processo);
            }
        }
    }
}

//Função que executa a instrução ZER
void executeZERPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual){
    int variavel = mem[ (paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead + 1];
    int paginaVariaveis = variablesPaging(pagingAssist, paginaAtual);
    mem[ (paginaVariaveis*PAGINA) + variavel] = 0;
    nextLinePaging(pagingAssist, paginaAtual);
}

//Função que executa a instrução INC
void executeINCPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual){
    int variavel = mem[ (paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead + 1];
    int paginaVariaveis = variablesPaging(pagingAssist, paginaAtual);
    mem[ (paginaVariaveis*PAGINA) + variavel]++;
    nextLinePaging(pagingAssist, paginaAtual);
}

//Função que executa a instrução DEC
void executeDECPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual){
    int variavel = mem[ (paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead + 1];
    int paginaVariaveis = variablesPaging(pagingAssist, paginaAtual);
    mem[ (paginaVariaveis*PAGINA) + variavel]--;
    nextLinePaging(pagingAssist, paginaAtual);
}

//Função que executa a instrução FRK
void executeFRKPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual, int *processoAtual, struct Queue* ready){
	int variavel = mem[ (paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead + 1];
    nextLinePaging(pagingAssist, paginaAtual);
    int processo = pagingAssist[paginaAtual].processo, iAnterior = 0;
    bool iAnteriorInicializado = false;
    for(int i=0; i<PAGASSIST; i++){
        if(processo == pagingAssist[i].processo){
            int k=0;
            while(k<PAGASSIST){
                if(pagingAssist[k].processo == -1)
                    break;
                k++;
            }
            pagingAssist[k] = pagingAssist[i];
            pagingAssist[k].processo = *processoAtual;
            if(!pagingAssist[k].variaveis)
                pagingAssist[k].backPage = iAnterior;
            if(!iAnteriorInicializado)
           		iAnteriorInicializado = true;
            else
                pagingAssist[iAnterior].nextPage = k;
            iAnterior = k;
            for(int j=0; j<PAGINA; j++)
                mem[(k*PAGINA) + j] = mem[(i*PAGINA) + j];
    		if(pagingAssist[i].variaveis){
    			//Coloca no pai o PID do filho
    			mem[(i*PAGINA) + variavel] = *processoAtual;
    			//Colocar no filho o valor 0
            	mem[(k*PAGINA) + variavel] = 0;
            }
        }
    }
    enQueue(ready, *processoAtual);
    *processoAtual+=1;
}

//Função que executa a instrução JFW
void executeJFWPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual){
    int jump = mem[ (paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead + 1]*2;
    if(jump == 0)
        nextLinePaging(pagingAssist, paginaAtual);
    else{
        bool loop = true;
        while(loop){
            if(pagingAssist[paginaAtual].lineToRead + jump >= PAGINA){
                jump-=(PAGINA - pagingAssist[paginaAtual].lineToRead);
                pagingAssist[paginaAtual].terminou = true;
                pagingAssist[paginaAtual].lineToRead = 8;
                paginaAtual = pagingAssist[paginaAtual].nextPage;
                pagingAssist[paginaAtual].lineToRead = 0;
            }else{
                pagingAssist[paginaAtual].lineToRead+=jump;
                break;
            }
        }
    }
}

//Função que executa a instrução JBK
void executeJBKPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual){
    int jump = mem[ (paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead + 1]*2;
    if(jump == 0)
        nextLinePaging(pagingAssist, paginaAtual);
    else{
        bool loop = true;
        while(loop){
            if(pagingAssist[paginaAtual].lineToRead - jump < 0){
                jump-=(pagingAssist[paginaAtual].lineToRead);
                pagingAssist[paginaAtual].lineToRead = 0;
                paginaAtual = pagingAssist[paginaAtual].backPage;
                pagingAssist[paginaAtual].lineToRead = PAGINA;
                pagingAssist[paginaAtual].terminou = false;
            }else{
                pagingAssist[paginaAtual].lineToRead-=(jump);
                break;
            }
        }
    }
}

//Função que executa a instrução DSK
void executeDSKPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual, int timeBlocked[MAXPROCESSOS], struct Queue* run, struct Queue* blocked){
    timeBlocked[pagingAssist[paginaAtual].processo] = 5;
    enQueue(blocked, deQueue(run));
    nextLinePaging(pagingAssist, paginaAtual);
}

//Função que executa a instrução JIZ
void executeJIZPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual){
    int variavel = mem[ (paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead + 1];
    int paginaVariaveis = variablesPaging(pagingAssist, paginaAtual);
    int number = mem[ (paginaVariaveis*PAGINA) + variavel];
    pagingAssist[paginaAtual].lineToRead+=2;
    if(pagingAssist[paginaAtual].lineToRead>=PAGINA){
        pagingAssist[paginaAtual].terminou = true;
        paginaAtual = pagingAssist[paginaAtual].nextPage;
    }
    if(number == 0)
        nextLinePaging(pagingAssist, paginaAtual);
}

//Função que executa a instrução PRT
void executePRTPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual, int *standOutput, bool *standOutputValid){
    int variavel = mem[ (paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead + 1];
    int paginaVariaveis = variablesPaging(pagingAssist, paginaAtual);
    *standOutput = mem[ (paginaVariaveis*PAGINA) + variavel];
    *standOutputValid = true;
    nextLinePaging(pagingAssist, paginaAtual);
}

//Função que remove um processo
void removerProcessoPaging(struct pagingElements pagingAssist[PAGASSIST], int processo){
	int paginaAtual;
    for(paginaAtual=0; paginaAtual<PAGASSIST; paginaAtual++){
        if(pagingAssist[paginaAtual].processo == processo)
            pagingAssist[paginaAtual].processo = -1;
    }
}

//Função que executa a instrução HLT
void executeHLTPaging(struct pagingElements pagingAssist[PAGASSIST], struct Queue* run){
   removerProcessoPaging(pagingAssist, deQueue(run));
}

//Função que verifica se a instrução FRK é possível
bool checkFRKPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual){
    int processo = pagingAssist[paginaAtual].processo, count=0;
    for(int i=0; i<PAGASSIST; i++){
        if(pagingAssist[i].processo == processo)
            count++;
    }
    if(countSpacePaging(pagingAssist) >= count)
        return false;
    else{
    	pagingAssist[paginaAtual].lineToRead+=2;
    	if(pagingAssist[paginaAtual].lineToRead>=PAGINA)
        	pagingAssist[paginaAtual].terminou = true;
        printf("fork sem sucesso\n");
    }
    return true;
}

//Função que verifica se a instrução JFW é possível
bool checkJumpLimitFowardPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual){
    int jump = mem[(paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead + 1]*2;
    int processo = pagingAssist[paginaAtual].processo;
    bool loop = true;
    while(loop){
        if(pagingAssist[paginaAtual].lineToRead + jump >= PAGINA){
            jump-=(PAGINA - pagingAssist[paginaAtual].lineToRead);
            paginaAtual = pagingAssist[paginaAtual].nextPage;
            if(paginaAtual == -1){
                printf("erro de segmentação do processo %d\n", processo);
                return true;
            }
            if(processo != pagingAssist[paginaAtual].processo){
            	printf("erro de segmentação do processo %d\n", processo);
                return true;
            }
        }else
            break;
    }
    if (mem[ (paginaAtual*PAGINA) + jump] == -1){
        printf("erro de segmentação do processo %d\n", processo);
        return true;
    }
    return false;
}

//Função que verifica se a instrução JIZ é possível
bool checkJizFowardPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual){
    int variavel = mem[ (paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead + 1];
    int paginaVariaveis = variablesPaging(pagingAssist, paginaAtual);
    int number = mem[ (paginaVariaveis*PAGINA) + variavel];
    int jump, processo = pagingAssist[paginaAtual].processo, line;
    bool loop = true, firstloop = true;
    if(number == 0)
        jump=4;
    else
        jump=2;
    while(loop){
        if(firstloop)
            line = pagingAssist[paginaAtual].lineToRead;
        else
            line = PAGINA-1;
        if(line + jump >= PAGINA){
            jump-=(PAGINA - line);
            paginaAtual = pagingAssist[paginaAtual].nextPage;
            if(paginaAtual == -1){
                printf("erro de segmentação do processo %d\n", processo);
                return true;
            }
            if(processo != pagingAssist[paginaAtual].processo){
            	printf("erro de segmentação do processo %d\n", processo);
                return true;
            }
        }else
            break;
    }
    if (mem[ (paginaAtual*PAGINA) + jump] == -1){
        printf("erro de segmentação do processo %d\n", processo);
        return true;
    }
    return false;
}

//Função que verifica se a instrução JBK é possível
bool checkJumpLimitBackPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual){
    int jump = mem[(paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead + 1]*2;
    int processo = pagingAssist[paginaAtual].processo;
    bool loop = true, exception = false;
    while(loop){
        if(pagingAssist[paginaAtual].lineToRead - jump < 0){
            jump-=(pagingAssist[paginaAtual].lineToRead + 1);
            paginaAtual = pagingAssist[paginaAtual].backPage;
            if(paginaAtual == -1)
                exception = true;
            else if(pagingAssist[paginaAtual].variaveis)
                exception = true;
            if(exception){
                printf("erro de segmentação do processo %d\n", processo);
                return true;
            }
            if(processo != pagingAssist[paginaAtual].processo){
            	printf("erro de segmentação do processo %d\n", processo);
                return true;
            }
        }else
            break;
    }
    if (mem[ pagingAssist[paginaAtual].lineToRead - jump - 1] == -1){
        printf("erro de segmentação do processo %d\n", processo);
        return true;
    }
    return false;
}

//Função que verifica se a variável que a instrução utiliza é possível
bool checkVarAccessPaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual) {
    int variavel = mem[ (paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead + 1];
    int paginaVariaveis = variablesPaging(pagingAssist, paginaAtual);
    int number = mem[ (paginaVariaveis*PAGINA) + variavel];
    if (number > 9 || number < 0){
        printf("erro de segmentação do processo %d\n", pagingAssist[paginaAtual].processo);
        return true;
    }
    return false;           
}

//Função que verifica se a instrução pertence ao processo
bool checkLimitRangePaging(int mem[MEMSIZE], struct pagingElements pagingAssist[PAGASSIST], int paginaAtual){
    if (mem[(paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead] == -1){
        printf("erro de segmentação do processo %d\n", pagingAssist[paginaAtual].processo);
        return true;
    }
    return false;
}

//Função que retorna a página atual do processo
int findPaginaAtual(struct pagingElements pagingAssist[PAGASSIST], int processo){
	int paginaAtual;
	for(paginaAtual=0; paginaAtual<PAGASSIST; paginaAtual++){
        if(pagingAssist[paginaAtual].processo == processo && !pagingAssist[paginaAtual].variaveis) {
            if(!pagingAssist[paginaAtual].terminou)
                break;
        }
    }
    return paginaAtual;
}

//Função que verifica todas as exceções da execução do programa
bool checkExceptionPaging(int mem[MEMSIZE], struct Queue* run, struct pagingElements pagingAssist[PAGASSIST], int processoAtual, bool *checkFRK){
    int paginaAtual = findPaginaAtual(pagingAssist, inicio(run));
    if(paginaAtual>=PAGASSIST)
		return true;
    if(checkLimitRangePaging(mem, pagingAssist, paginaAtual))
        return true;
    switch(mem[(paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead]){
        case 0:
            if(checkVarAccessPaging(mem, pagingAssist, paginaAtual))
                return true;
            break;
        case 1:
            if(checkVarAccessPaging(mem, pagingAssist, paginaAtual))
                return true;
            break;
        case 2:
            if(checkVarAccessPaging(mem, pagingAssist, paginaAtual))
                return true;
            break;
        case 3:
            if(checkFRKPaging(mem, pagingAssist, paginaAtual)){
            	*checkFRK = true;
                return true;
            }
            break;
        case 4:
            if(checkJumpLimitFowardPaging(mem, pagingAssist, paginaAtual))
                return true;
            break;
        case 5:
            if(checkJumpLimitBackPaging(mem, pagingAssist, paginaAtual))
                return true;
            break;
        case 7:
            if(checkJizFowardPaging(mem, pagingAssist, paginaAtual))
                return true;
            break;
        case 8:
            if(checkVarAccessPaging(mem, pagingAssist, paginaAtual))
                return true;
            break;
        default:
            return false;
            break;
    }
    return false;
}


//Função que executa as instruções antes da impressão do programa
bool executeRunBeforePaging(int mem[MEMSIZE], struct Queue* ready, struct Queue* run, struct Queue* blocked, struct pagingElements pagingAssist[PAGASSIST], int timeBlocked[MAXPROCESSOS], int *processoAtual, int *standOutput, bool *standOutputValid){
    int paginaAtual = findPaginaAtual(pagingAssist, inicio(run));
    switch(mem[(paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead]){
        case 0:
            executeZERPaging(mem, pagingAssist, paginaAtual);
            return true;
        case 1:
            executeINCPaging(mem, pagingAssist, paginaAtual);
            return true;
        case 2:
            executeDECPaging(mem, pagingAssist, paginaAtual);
            return true;
        case 3:
            executeFRKPaging(mem, pagingAssist, paginaAtual, processoAtual, ready);
            return true;
        case 4:
            executeJFWPaging(mem, pagingAssist, paginaAtual);
            return true;
        case 5:
            executeJBKPaging(mem, pagingAssist, paginaAtual);
            return true;
        case 7:
            executeJIZPaging(mem, pagingAssist, paginaAtual);
            return true;
        case 8:
            executePRTPaging(mem, pagingAssist, paginaAtual, standOutput, standOutputValid);
            return true;
        default:
        	return false;
    }
}

//Função que executa as instruções depois da impressão do programa
void executeRunAfterPaging(int mem[MEMSIZE], struct Queue* ready, struct Queue* run, struct Queue* blocked, struct pagingElements pagingAssist[PAGASSIST], int timeBlocked[MAXPROCESSOS], int *processoAtual){
    int paginaAtual = findPaginaAtual(pagingAssist, inicio(run));
    switch(mem[(paginaAtual*PAGINA) + pagingAssist[paginaAtual].lineToRead]){
        case 6:
            executeDSKPaging(mem, pagingAssist, paginaAtual, timeBlocked, run, blocked);
            break;
        case 9:
            executeHLTPaging(pagingAssist, run);
            break;
    }
}

//Função que executa o programa a partir da paginação
void executionPaging(int linhas, int inputsave[linhas][2]){
    int instante=0, INIAtual=0, processoAtual=1, standOutput=-1, quantumTimer = QUANTUM;
    int mem[MEMSIZE];

    struct pagingElements pagingAssist[PAGASSIST];
    inicializePaging(pagingAssist);
    int timeBlocked[MAXPROCESSOS];
    startIntArray(mem, MEMSIZE);
    startIntArray(timeBlocked, MAXPROCESSOS);

    struct Queue* ready = createQueue(linhas);
    struct Queue* run = createQueue(1);
    struct Queue* blocked = createQueue(linhas);
    
    while(SizeQueue(ready)>0 || SizeQueue(run)>0 || SizeQueue(blocked)>0 || INIAtual<linhas){
        bool standOutputValid = false, checkFRK = false;
        entrarProcessoPaging(linhas, inputsave, mem, pagingAssist, ready, instante, &INIAtual, &processoAtual);
        if(quantumTimer <= 0){
            enQueue(ready, deQueue(run));
            quantumTimer = QUANTUM;
        }
        ready_run(ready, run);
        if(isEmpty(run)){
            blocked_readyPaging(blocked, ready, timeBlocked);
            ready_run(ready, run);
            if(!isEmpty(run)){
            	if(!checkExceptionPaging(mem, run, pagingAssist, processoAtual, &checkFRK)){
                    if(executeRunBeforePaging(mem, ready, run, blocked, pagingAssist, timeBlocked, &processoAtual, &standOutput, &standOutputValid)){
                    	prints(instante, ready, run, blocked, standOutput, standOutputValid);
                    }else{
                    	prints(instante, ready, run, blocked, standOutput, standOutputValid);
                    	executeRunAfterPaging(mem, ready, run, blocked,  pagingAssist, timeBlocked, &processoAtual);
                    }
                }else{
                	if(!checkFRK)
                		removerProcessoPaging(pagingAssist, deQueue(run));
                    instante--;
                }
            }else
                prints(instante, ready, run, blocked, standOutput, standOutputValid);
        }else{
            if(!checkExceptionPaging(mem, run, pagingAssist, processoAtual, &checkFRK)){
                blocked_readyPaging(blocked, ready, timeBlocked);
                if(executeRunBeforePaging(mem, ready, run, blocked, pagingAssist, timeBlocked, &processoAtual, &standOutput, &standOutputValid)){
                	prints(instante, ready, run, blocked, standOutput, standOutputValid);
                }else{
                	prints(instante, ready, run, blocked, standOutput, standOutputValid);
                	executeRunAfterPaging(mem, ready, run, blocked,  pagingAssist, timeBlocked, &processoAtual);
            	}
            }else{
            	if(!checkFRK)
                	removerProcessoPaging(pagingAssist, deQueue(run));
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