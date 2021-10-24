//Função que altera no array todas as posições para 0, dependente da quantidade
void startIntArray(int array[], int quantidade){
    for(int i=0; i<quantidade; i++)
        array[i]=0;
}

//Função que encontra a próxima instrução INI
void NextINI(int linhas, int inputsave[linhas][2], int *INIAtual){
    *INIAtual+=1;
    while(*INIAtual<linhas){
        if(inputsave[*INIAtual][0]==-1){
            break;
        }
        *INIAtual+=1;
    }

}

//Função que coloca um processo do estado Ready para o estado Run se o estado Run estiver vazio
void ready_run(struct Queue* ready, struct Queue* run){
    if(!isEmpty(ready) && !isFull(run))
        enQueue(run, deQueue(ready));
}

//Função que imprime a execução do programa
void prints(int instante, struct Queue* ready, struct Queue* run, struct Queue* blocked, int standOutput, bool standOutputValid){
    printf("%d | ", instante);
    if(standOutputValid)
        printf("%d | ", standOutput);
    else
        printf("  | ");
    if(isEmpty(ready)){
        printf("   | ");
    }else{
        for(int i=0; i<SizeQueue(ready); i++){
            int x = deQueue(ready);
            printf("P%d ", x);
            enQueue(ready, x);
        }
        printf("| "); 
    }
    if(SizeQueue(run)>0)
        printf("P%d | ", inicio(run));
    else
        printf("   | ");
    for(int i=0; i<SizeQueue(blocked); i++){
        int y = deQueue(blocked);
        printf("P%d ", y);
        enQueue(blocked, y);
    }
    printf("\n");
}