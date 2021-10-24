//Função que retorna o número de linhas do ficherio que recebe
int contalinhas(char nome[SIZE]){
    FILE *f;
    int linhas=1;
    char a;
    if((f=fopen(nome,"r")) == NULL){
        printf("Ficheiro não existe!\n");
        exit(1);
    }
    for (a=getc(f); a!=EOF; a=getc(f)){
        if (a == '\n')
            linhas+=1;
    }
    fclose(f);
    return linhas;
}

//Função que coloca a informação do ficheiro para o inputsave
void lerinput(int linhas, int inputsave[linhas][2], FILE *f){
    char line[SIZE], cut[SIZE];
    int index=0;
    while(fgets(line, sizeof(line), f) != NULL){
        int inicio=0, info=0;
        for(int i=0; i<strlen(line); i++){
            if(!(line[i]==' ' && line[i+1]==' ')){
                if(line[i]==' ' || (line[i]=='\n' && line[i-2]!=' ')){
                    memset(cut, '\0', sizeof(cut));
                    if(info==0){
                        strncpy(cut, line, i);
                        if(strcmp(cut, "INI") == 0){
                            inputsave[index][0]=-1;
                        }else if(strcmp(cut, "ZER") == 0){
                            inputsave[index][0]=0;
                        }else if(strcmp(cut, "INC") == 0){
                            inputsave[index][0]=1;
                        }else if(strcmp(cut, "DEC") == 0){
                            inputsave[index][0]=2;
                        }else if(strcmp(cut, "FRK") == 0){
                            inputsave[index][0]=3;
                        }else if(strcmp(cut, "JFW") == 0){
                            inputsave[index][0]=4;
                        }else if(strcmp(cut, "JBK") == 0){
                            inputsave[index][0]=5;
                        }else if(strcmp(cut, "DSK") == 0){
                            inputsave[index][0]=6;
                        }else if(strcmp(cut, "JIZ") == 0){
                            inputsave[index][0]=7;
                        }else if(strcmp(cut, "PRT") == 0){
                            inputsave[index][0]=8;
                        }else if(strcmp(cut, "HLT") == 0){
                            inputsave[index][0]=9;
                        }
                        info++;
                    }else{
                        strncpy(cut, line+inicio, i);
                        inputsave[index][1]=atoi(cut);
                    }
                    inicio = i;
                }
            }
        }
        index++;
    }
} 

//Função que abre o ficheiro e chama o método lerinput:
void openfileteste(int linhas, int inputsave[linhas][2], char nome[SIZE]){
    FILE *f;
    if((f = fopen(nome,"r")) == NULL)
    	exit(1);
    lerinput(linhas, inputsave, f);
}