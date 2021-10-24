#include <stdio.h>
#include <stdbool.h>
#include <string.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h>
	
#define TRUE 1 
#define FALSE 0
#define MAXCLIENTS 5
#define PORT 5555
#define MAXSIZENICK 9
#define MAXSIZEPASS 9
#define MAXSIZEMETHOD 4
#define MAXSIZEINPUT 512
#define MAXSIZEINPUTCONTEXT 550
#define MAXSIZERPLY 500
#define MAXSIZEMSSG 700
#define MAXSIZESPLITS 10
#define DATABASE "database.txt"

#define MAXSIZECHANNEL 20
#define CHANNELSSIZE 3
#define CHANNELS {"default", "cn", "oss"}


// struct client
struct client{
	char nick[MAXSIZENICK+1];
	char pass[MAXSIZEPASS+1];
	char channel[MAXSIZECHANNEL+1];
	bool isAuthenticated;
	bool isOperator;
	int socket;
};

// struct replyPacket
struct replyPacket{
	char rply[MAXSIZERPLY+1];
	char serverMessageGlobal[MAXSIZEMSSG+1];
	char serverMessageChannel[MAXSIZEMSSG+1];
	char serverChannelName[MAXSIZECHANNEL+1];
	char serverMessageChannelPrevious[MAXSIZEMSSG+1];
	char serverChannelNamePrevious[MAXSIZECHANNEL+1];
};

// initializereplyPacket new reply
struct replyPacket initializereplyPacket(){
	struct replyPacket packet;
	strcpy(packet.rply, "");
	strcpy(packet.serverMessageGlobal, "");
	strcpy(packet.serverMessageChannel, "");
	strcpy(packet.serverChannelName, "");
	strcpy(packet.serverMessageChannelPrevious, "");
	strcpy(packet.serverChannelNamePrevious, "");
	return packet;
}

// printPacket prints packet
void printPacket(struct replyPacket packet){
	printf("rply: %s\n",packet.rply);
	printf("serverMessageGlobal: %s\n", packet.serverMessageGlobal);
	printf("serverMessageChannel: %s\n", packet.serverMessageChannel);
	printf("serverChannelName: %s\n", packet.serverChannelName);
	printf("serverMessageChannelPrevious: %s\n", packet.serverMessageChannelPrevious);
	printf("serverChannelNamePrevious: %s\n", packet.serverChannelNamePrevious);
	printf("----------------------\n");
}

// splitStrings splits input by spaces and returns it
void splitStrings(char input[], char split[][MAXSIZEINPUT]){
	int words = 0, wordIndex = 0;
	for(int i = 0; i < strlen(input) ; i++){
		if (input[i] == ' ' || input[i] == '\0'){
			split[words][wordIndex] = '\0';
			words++;
			wordIndex = 0;
		} else {
			split[words][wordIndex] = input[i];
			wordIndex++;
		}
	}
}

// initializeClientDB initializes ClientDB
void initializeClientDB(struct client ClientDB[]){

	strcpy(ClientDB[0].nick, "ADMIN");
	strcpy(ClientDB[0].pass, "admin");
	strcpy(ClientDB[0].channel, "default");
	ClientDB[0].isAuthenticated = false;
	ClientDB[0].isOperator = true;
	ClientDB[0].socket = -1;
	for (int i = 1; i < MAXCLIENTS; i++){
		strcpy(ClientDB[i].nick, "");
		strcpy(ClientDB[i].pass, "");
		strcpy(ClientDB[i].channel, "default");
		ClientDB[i].isAuthenticated = false;
		ClientDB[i].isOperator = false;
		ClientDB[i].socket = -1;
	}
	FILE *stream;
	char *line = NULL;
	size_t len = 0;
	ssize_t readline;
	int i = 0;

	stream = fopen(DATABASE, "r");

	if(!stream){
		return;
	}
	while ((readline = getline(&line, &len, stream)) != -1) {
		char split[5][MAXSIZEINPUT];
		splitStrings(line, split);
		strcpy(ClientDB[i].nick, split[0]);

		if (strcmp(split[1], "-")==0)
			strcpy(ClientDB[i].pass, "");
		else
			strcpy(ClientDB[i].pass, split[1]);

		strcpy(ClientDB[i].channel, split[2]);
		ClientDB[i].isAuthenticated = false;

		if (strcmp(split[3], "false")==0)
			ClientDB[i].isOperator = false;
		else
			ClientDB[i].isOperator = true;

		ClientDB[i].socket = -1;
		i++;
	}
	free(line);
	fclose(stream);
};

// saveClientDB saves ClientDB in file
void saveClientDB(struct client ClientDB[]){
	FILE *file = fopen(DATABASE, "w");

	for(int i = 0; i< MAXCLIENTS; i++){
		if(strcmp(ClientDB[i].nick, "")!=0){
			fprintf(file, "%s ", ClientDB[i].nick);
			if (strcmp(ClientDB[i].pass, "")==0)
				fprintf(file, "- ");
			else
				fprintf(file, "%s ", ClientDB[i].pass);

			fprintf(file, "%s ", ClientDB[i].channel);

			if (ClientDB[i].isOperator)
				fprintf(file, "true ");
			else
				fprintf(file, "false ");
			fprintf(file, "\n");
		}	
	}
	fclose(file);
}

// printClientDB prints ClientDB
void printClientDB(struct client ClientDB[]){
	for (int i = 0; i < MAXCLIENTS; i++){
		printf("nick: %s\n", ClientDB[i].nick);
		printf("pass: %s\n", ClientDB[i].pass);
		printf("channel: %s\n", ClientDB[i].channel);
		printf("isAuthenticated: %d\n", ClientDB[i].isAuthenticated);
		printf("isOperator: %d\n", ClientDB[i].isOperator);
		printf("socket: %d\n", ClientDB[i].socket);
	}
	printf("----------------------\n");
};

// setClient sets client in index
void setClient(struct client client, int index, struct client ClientDB[]){
	strcpy(ClientDB[index].nick, client.nick);
	strcpy(ClientDB[index].pass,client.pass);
	strcpy(ClientDB[index].channel, client.channel);
	ClientDB[index].isAuthenticated = client.isAuthenticated;
	ClientDB[index].isOperator = client.isOperator;
	ClientDB[index].socket = client.socket;
}

// startupClient startup new client by nick and socket
void startupClient(char nick[], int socket, int index, struct client ClientDB[]){
	strcpy(ClientDB[index].nick, nick);
	ClientDB[index].socket = socket;
}

// findClientStruct find and return index of client in ClientDB
int findClientStructByNick(char nick[], struct client ClientDB[]){
	for(int i = 0; i < MAXCLIENTS; i++){
		if (strcmp(ClientDB[i].nick, nick) == 0)
			return i;
	}
	return -1;
}

// findClientStruct find and return index of client in ClientDB
int findClientStructBySocket(int socket,  struct client ClientDB[]){
	for(int i = 0; i < MAXCLIENTS; i++){
		if (ClientDB[i].socket == socket)
			return i;
	}
	return -1;
}

// findEmptySpace find and return index of space in ClientDB
int findEmptySpace(struct client ClientDB[]){
	for(int i = 0; i < MAXCLIENTS; i++){
		if (strcmp(ClientDB[i].nick, "") == 0)
			return i;
	}
	return -1;
}

// isValidNick checks if nick is valid
bool isValidNick(char input[]){
	if (strlen(input) > MAXSIZENICK)
		return false;
	for (int i = 0; i < strlen(input); i++){
		if ( (int)input[i]<48 || (57<(int)input[i] && (int)input[i]<65) || (90<(int)input[i] && (int)input[i]<97) || (int)input[i]>122 )
			return false;
	}
	return true;
}

// isOnline checks if nick is online
bool isOnline(char input[], struct client ClientDB[]){
	for (int i = 0; i < MAXCLIENTS; i++){
		if (strcmp(input, ClientDB[i].nick) == 0){
			if (ClientDB[i].socket >= 0)
				return true;
		}
	}
	return false;
}

// isChannelValid checks if channel is valid
bool isChannelValid(char input[]){
	for(int i=0; i < CHANNELSSIZE; i++){
		char aux[CHANNELSSIZE][MAXSIZECHANNEL]=CHANNELS;
		if (strcmp(input, aux[i])==0)
			return true;
	}
	return false;
}

// checkClientInDB checks if methods can be used
bool checkClientInDB(char method[], int index, struct client ClientDB[]){
	if (ClientDB[index].socket < 0 && strcmp(method, "NICK") != 0)
		return false;
	return true;
}

// allChannels put in aux all channels in a string
void allChannels(char * aux){
	char channels[CHANNELSSIZE][MAXSIZECHANNEL]=CHANNELS;
	strcat(aux, channels[0]);
	for(int i=1; i < CHANNELSSIZE; i++){
		strcat(aux, ";");
		strcat(aux, channels[i]);
	}
}

// clientStatus put in aux client status
void clientStatus(char * aux, struct client client){
	if (client.isOperator == true) {
		strcat(aux, "Operador");
	} else if (client.isAuthenticated == true) {
		strcat(aux, "Registado");
	} else 
		strcat(aux, "Não Registado");
}

// allWhos returns all users in the channel in a string
void allWhos(char * aux, struct client client, struct client ClientDB[]){
	bool findFirst = false;
	for(int i=0; i < MAXCLIENTS; i++){
		if (strcmp(ClientDB[i].channel, client.channel)== 0 && isOnline(ClientDB[i].nick, ClientDB)){
			if (findFirst)
				strcat(aux, ";");
			strcat(aux, ClientDB[i].nick);
			strcat(aux, "/");
			clientStatus(aux, ClientDB[i]);
			findFirst = true;
		}
	}
}

// Comandos de utilizador (todos os utilizadores):
// nick method to check all conditions 
int nick(char input[], int socket, struct client ClientDB[]){
	if (input == ""){
		return 2;
	} else if (!isValidNick(input)){
		return 3;
	} else if (isOnline(input, ClientDB)){
		return 4;
	}
	int removeIndex = findClientStructBySocket(socket, ClientDB);
	if (removeIndex >= 0){
		ClientDB[removeIndex].socket = -1;
		ClientDB[removeIndex].isAuthenticated = false;
	}
		
	int index = findClientStructByNick(input, ClientDB);
	if (index == -1)
		index = findEmptySpace(ClientDB);
	startupClient(input, socket, index, ClientDB);
	return 1;	
}

// mssg method to check all conditions
int mssg(char input[], struct client client){
	if (input == ""){
		return "", 2;
	}else if (strlen(input) > MAXSIZEINPUT){
		return "", 3;
	}
	return 1;
}

// Comandos de utilizador registado (pode ser operador ou não):
// pass method to check all conditions
int pass(char input[], struct client client, struct client ClientDB[]){
	if (strcmp(client.pass, "") == 0){
		return 2;
	}else if (strcmp(input, client.pass) != 0){
		return 3;
	}
	int index = findClientStructByNick(client.nick, ClientDB);
	ClientDB[index].isAuthenticated = true;
	return 1;
}

// join method to check all conditions
int join(char input[], struct client client, struct client ClientDB[]){
	if (!isChannelValid(input)){
		return 2;
	} else if(client.isAuthenticated == false){
		return 3;
	}
	int index = findClientStructByNick(client.nick, ClientDB);
	strcpy(ClientDB[index].channel, input);
	return 1;
}

// list method to check all conditions
int list(struct client client){
	if (client.isAuthenticated == false){
		return 2;
	}
	return 1;
}

// whos method to check all conditions
int whos(struct client client){
	if (client.isAuthenticated == false){
		return 2;
	}
	return 1;
}

// Comandos de Operador:
// kick method to check all conditions
int kick(char input[], struct client client, struct client ClientDB[]){
	if (!client.isOperator){
		return 2;
	} else if (!client.isAuthenticated) {
		return 3;
	}
	int index = findClientStructByNick(input, ClientDB);
	if (index < 0 || strcmp(ClientDB[index].pass, "")==0)
		return 4;
	strcpy(ClientDB[index].pass, "");
	ClientDB[index].isAuthenticated = false;
	ClientDB[index].isOperator = false;
	return 1;

}

// regs method to check all conditions
int regs(char input[], struct client client, struct client ClientDB[]){
	char split[MAXSIZESPLITS][MAXSIZEINPUT];
	splitStrings(input, split);
	if (!client.isOperator){
		return 2;
	} else if (!client.isAuthenticated) {
		return 3;
	}
	int index = findClientStructByNick(split[0], ClientDB);
	if (index >= 0){
		if (strcmp(ClientDB[index].pass, "") == 0){
			strcpy(ClientDB[index].pass, split[1]);
			return 1;
		}
		return 4;
	}
	index = findEmptySpace(ClientDB);
	strcpy(ClientDB[index].nick, split[0]);
	strcpy(ClientDB[index].pass, split[1]);
	return 1;
}

// oper method to check all conditions
int oper(char input[], struct client client, struct client ClientDB[]){
	if (!client.isOperator){
		return 2;
	} else if (!client.isAuthenticated) {
		return 3;
	}
	int index = findClientStructByNick(input, ClientDB);
	if (index >= 0){
		if (strcmp(ClientDB[index].pass, "") != 0){
			if (ClientDB[index].isOperator == false){
				ClientDB[index].isOperator = true;
				return 1;
			}
			return 5;
		}
	}
	return 4;
}

// quit method to check all conditions
int quit(char input[], struct client client, struct client ClientDB[]) {
	if (!client.isOperator){
		return 2;
	} else if (!client.isAuthenticated) {
		return 3;
	}
	int index = findClientStructByNick(client.nick, ClientDB);
	ClientDB[index].isOperator = false;
	return 1;
} 

// methodSelector for return replyPacket with information
struct replyPacket methodSelector(char input[], int socket, struct client ClientDB[]){
	
	struct replyPacket packet = initializereplyPacket();
	int number, index;
	char channel[MAXSIZECHANNEL];

	index = findClientStructBySocket(socket, ClientDB);
	if (index == -1)
		index = findEmptySpace(ClientDB);
	printf("second index: %d\n", index);
	struct client client = ClientDB[index];

	// method will be a char to choose which method is
	char method[MAXSIZEMETHOD+1], inputContext[MAXSIZEINPUTCONTEXT+1];
	memset(method, '\0', sizeof(method));
	memset(inputContext, '\0', sizeof(inputContext));
	strncpy(method, input, 4);
	strncpy(inputContext, input+5, strlen(input));

	if (!checkClientInDB(method, index, ClientDB)) {
		strcpy(packet.rply, "RPLY 1002 – Erro. Coloque o NICK.\n");
		return packet;
	} else if (strcmp(method, "NICK") == 0) {
		number = nick(inputContext, socket, ClientDB);
		switch (number){
		case 2:
			strcpy(packet.rply,  "RPLY 002 - Erro: Falta introdução do nome.\n");
			return packet;
		case 3:
			strcpy(packet.rply, "RPLY 003 - Erro: Nome pedido não válido (caso não tenha apenas letras e algarismos e 9 caracteres no máximo)\n");
			return packet;
		case 4:
			strcpy(packet.rply, "RPLY 004 - Erro: nome já em uso (num outro utilizador registado ou em uso por um utilizador não registado, e o comando não tem qualquer efeito\n");
			return packet;
		default:
			{
				strcpy(packet.rply, "RPLY 001 - Nome atribuído com sucesso\n");
				char str1[] = "server :> novo utilizador ";
				strcat(str1, inputContext);
				strcat(str1, "\n");
				strcpy(packet.serverMessageGlobal, str1);
				return packet;
			}
		}
	} else if (strcmp(method, "MSSG") == 0) {
		number = mssg(inputContext, client);
		switch (number){
		case 2:
			strcpy(packet.rply, "RPLY 102 - Erro. Não há texto na mensagem.\n");
			return packet;
		case 3:
			strcpy(packet.rply, "RPLY 103 - Erro. Mensagem demasiado longa.\n");
			return packet;
		default:
			{
				strcpy(packet.rply, "RPLY 101 - mensagem enviada com sucesso.\n");
				char str1[] = "";
				strcat(str1, client.nick);
				strcat(str1, " :> ");
				strcat(str1, inputContext);
				strcat(str1, "\n");
				strcpy(packet.serverMessageChannel, str1);
				strcpy(packet.serverChannelName, client.channel);
				return packet;
			}
		}
	} else if (strcmp(method, "PASS") == 0){
		number = pass(inputContext, client, ClientDB); 
		switch (number){
		case 2:
			strcpy(packet.rply, "RPLY 202 - Erro. Nome não está definido.\n");
			return packet;
		case 3:
			strcpy(packet.rply, "RPLY 203 - Erro. Password incorreta.\n");
			return packet;
		default:
			strcpy(packet.rply, "RPLY 201 - Autenticação com sucesso.\n");
			return packet;
		}
	} else if (strcmp(method, "JOIN") == 0) {
		char serverChannelNamePrevious[] = "";
		strcpy(serverChannelNamePrevious, client.channel);
		number = join(inputContext, client, ClientDB);
		switch (number){
		case 2:
			strcpy(packet.rply, "RPLY 302 – Erro. canal não existente\n");
			return packet;
		case 3:
			strcpy(packet.rply, "RPLY 303 - Erro. Não pode mudar para o canal (utilizador não autenticado)\n");
			return packet;
		default:
			{
				strcpy(packet.rply, "RPLY 301 - Mudança de canal com sucesso\n");
				strcpy(packet.serverChannelName, inputContext);
				strcpy(packet.serverChannelNamePrevious, serverChannelNamePrevious);

				char str1[] = "server :> ";
				strcat(str1, client.nick);
				strcat(str1, " entrou neste canal\n");
				strcpy(packet.serverMessageChannel, str1);

				char str2[] = "server :> ";
				strcat(str2, client.nick);
				strcat(str2, " deixou este canal\n");
				strcpy(packet.serverMessageChannelPrevious, str2);

				return packet;
			}
		}
	} else if (strcmp(method, "LIST") == 0){
		number = list(client);
		switch (number){
		case 2:
			strcpy(packet.rply, "RPLY 402 - Erro. Não pode ver canais (utilizador não autenticado)\n");
			return packet;
		default:
			{
				char str1[] = "RPLY 401 - ";
				allChannels(str1);
				strcat(str1, "\n");
				strcpy(packet.rply, str1);
				return packet;
			}
		}
	} else if (strcmp(method, "WHOS") == 0) {
		number = whos(client);
		switch (number){
		case 2:
			strcpy(packet.rply, "RPLY 502 - Erro. Não pode ver utilizadores (utilizador não autenticado)\n");
			return packet;
		default:
			{
				char str1[] = "RPLY 501 - ";
				allWhos(str1, client, ClientDB);
				strcat(str1, "\n");
				strcpy(packet.rply, str1);
				return packet;
			}	
		}
	} else if (strcmp(method, "KICK") == 0) {
		number = kick(inputContext, client, ClientDB);
		switch (number) {
		case 2: 
			strcpy(packet.rply, "RPLY 602 – Erro. Ação não autorizada, utilizador cliente não é um operador.\n");
			return packet;
		case 3:
			strcpy(packet.rply, "RPLY 603 – Erro. Ação não autorizada, utilizador cliente não está autenticado.\n");
			return packet;
		case 4:
			{
				char str1[] = "RPLY 604 – Erro. Ação não autorizada, utilizador registado ";
				strcat(str1, inputContext);
				strcat(str1, " não existe.\n");
				strcpy(packet.rply, str1);
				return packet;
			}
		default:
			{
				strcpy(packet.rply, "RPLY 601 – Utilizador expulso.\n");
				char str1[] = "server :> ";
				strcat(str1, inputContext);
				strcat(str1, " foi expulso\n");
				strcpy(packet.serverMessageGlobal, str1);
				return packet;
			}
		}
	} else if (strcmp(method, "REGS") == 0) {
		number = regs(inputContext, client, ClientDB);
		switch (number) {
		case 2: 
			strcpy(packet.rply, "RPLY 702 – Erro. Ação não autorizada, utilizador cliente não é um operador.\n");
			return packet;
		case 3:
			strcpy(packet.rply, "RPLY 703 – Erro. Ação não autorizada, utilizador cliente não está autenticado.\n");
			return packet;
		case 4:
			{
				char str1[] = "RPLY 703 – Erro. Ação não autorizada, utilizador ";
				char split[2][MAXSIZEINPUT];
				splitStrings(inputContext, split);
				strcat(str1, split[0]);
				strcat(str1, " já está registado.\n");
				strcpy(packet.rply, str1);
				return packet;
			}
		default:
			{
				strcpy(packet.rply, "RPLY 701 – Utilizador foi registado com sucesso.\n");
				char str1[] = "server :> ";
				strcat(str1, inputContext);
				strcat(str1, " foi registado\n");
				strcpy(packet.serverMessageGlobal, str1);
				return packet;
			}
			
		}
	} else if (strcmp(method, "OPER") == 0) {
		number = oper(inputContext, client, ClientDB);
		switch (number) {
		case 2: 
			strcpy(packet.rply, "RPLY 802 – Erro. Ação não autorizada, utilizador cliente não é um operador.\n");
			return packet;
		case 3:
			strcpy(packet.rply, "RPLY 803 – Erro. Ação não autorizada, utilizador cliente não está autenticado.\n");
			return packet;
		case 4:
			{
				char str1[] = "RPLY 804 – Erro. Ação não autorizada, utilizador ";
				strcat(str1, inputContext);
				strcat(str1, " não é um utilizador registado.\n");
				strcpy(packet.rply, str1);
				return packet;
			}
		case 5:
			{
				char str1[] = "RPLY 805 – Erro. Ação não autorizada, utilizador ";
				strcat(str1, inputContext);
				strcat(str1, " já é um operador.\n");
				strcpy(packet.rply, str1);
				return packet;
			}
		default:
			{
				strcpy(packet.rply, "RPLY 801 – Foi promovido a operador.\n");
				char str1[] = "server :> ";
				strcat(str1, inputContext);
				strcat(str1, " foi promovido a operador\n");
				strcpy(packet.serverMessageGlobal, str1);
				return packet;
			}
		}
	} else if (strcmp(method, "QUIT") == 0) {
		number = quit(inputContext, client, ClientDB);
		switch(number) {
		case 2: 
			strcpy(packet.rply, "RPLY 902 – Erro. Ação não autorizada, utilizador cliente não é um operador.\n");
			return packet;
		case 3:
			strcpy(packet.rply, "RPLY 903 – Erro. Ação não autorizada, utilizador cliente não está autenticado.\n");
			return packet;
		default:
			{
				strcpy(packet.rply, "RPLY 901 – Deixou de ser operador.\n");
				char str1[] = "server :> ";
				strcat(str1, inputContext);
				strcat(str1, " deixou de ser operador\n");
				strcpy(packet.serverMessageGlobal, str1);
				return packet;
			}	
		}
	} else {
		strcpy(packet.rply, "RPLY 1001 – Erro. Ação não reconhecida.\n");
		return packet;
	}
}

// bridge communicates the server with the clients
void bridge (int sd, int client_socket[], struct replyPacket packet, struct client ClientDB[]){
	send(sd , packet.rply , strlen(packet.rply) , 0);
	if (strcmp(packet.serverMessageGlobal, "") != 0){
		for (int i = 0; i < MAXCLIENTS; i++){
			if (ClientDB[i].socket >= 0)
				send(client_socket[ClientDB[i].socket], packet.serverMessageGlobal , strlen(packet.serverMessageGlobal) , 0);
		}
	}
	if (strcmp(packet.serverChannelName, "") != 0){
		for (int i = 0; i < MAXCLIENTS; i++){
			if (ClientDB[i].socket >= 0 && strcmp(ClientDB[i].channel, packet.serverChannelName) == 0)
				send(client_socket[ClientDB[i].socket], packet.serverMessageChannel , strlen(packet.serverMessageChannel) , 0);
		}
	}
	if (strcmp(packet.serverChannelNamePrevious, "") != 0){
		for (int i = 0; i < MAXCLIENTS; i++){
			if (ClientDB[i].socket >= 0 && strcmp(ClientDB[i].channel, packet.serverChannelNamePrevious) == 0)
				send(client_socket[ClientDB[i].socket], packet.serverMessageChannelPrevious , strlen(packet.serverMessageChannelPrevious) , 0);
		}
	}
}

// main function
int main(int argc , char *argv[]){

	int opt = TRUE; 
	int master_socket , addrlen , new_socket , client_socket[MAXCLIENTS], activity, i, valread, sd, max_sd; 
	struct sockaddr_in address;

	//put clients in database
	struct client ClientDB[MAXCLIENTS];
	initializeClientDB(ClientDB);

	//print & save database
	//printClientDB(ClientDB);
	saveClientDB(ClientDB);
		
	char buffer[1025]; 
		
	//set of socket descriptors 
	fd_set readfds; 
	
	//reset client_socket[] to 0
	for (i = 0; i < MAXCLIENTS; i++){ 
		client_socket[i] = 0; 
	} 
		
	//create a master socket 
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0){ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	
	//type of socket created 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(PORT); 
		
	//bind the socket to localhost port 5555 
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0){ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}

	printf("Listener on port %d \n", PORT); 
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(master_socket, 3) < 0){ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
	addrlen = sizeof(address); 
	puts("Waiting for connections ..."); 
		
	while(TRUE){
		//clear the socket set 
		FD_ZERO(&readfds); 

		//add master socket to set 
		FD_SET(master_socket, &readfds); 
		max_sd = master_socket; 
			
		//add child sockets to set 
		for ( i = 0 ; i < MAXCLIENTS ; i++){ 
			//socket descriptor 
			sd = client_socket[i]; 
				
			//if valid socket descriptor then add to read list 
			if(sd > 0) 
				FD_SET(sd , &readfds); 
				
			//highest file descriptor number, need it for the select function 
			if(sd > max_sd) 
				max_sd = sd; 
		} 
	
		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL); 
		if ((activity < 0) && (errno!=EINTR)){ 
			printf("select error"); 
		} 
			
		//If something happened on the master socket , 
		//then its an incoming connection 
		if (FD_ISSET(master_socket, &readfds)){ 
			if ((new_socket = accept(master_socket, 
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0){ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			
			//inform user of socket number - used in send and receive commands 
			printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs (address.sin_port));
				
			//add new socket to array of sockets 
			for (i = 0; i < MAXCLIENTS; i++) { 
				//if position is empty 
				if( client_socket[i] == 0 ) { 
					client_socket[i] = new_socket; 
					printf("Adding to list of sockets as %d\n" , i); 
					break; 
				} 
			} 
		} 
			
		//else its some IO operation on some other socket 
		for (i = 0; i < MAXCLIENTS; i++) {
			sd = client_socket[i]; 
			if (FD_ISSET(sd , &readfds)){ 
				//Check if it was for closing , and also read the 
				//incoming message 
				if ((valread = read(sd , buffer, 1024)) <= 0){ 
					//Somebody disconnected , get his details and print 
					getpeername(sd , (struct sockaddr*)&address , \ 
						(socklen_t*)&addrlen); 
					printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 

					//Close the socket and mark as 0 in list for reuse 
					//close( sd ); 
					client_socket[i] = 0;

					// Remove from ClientDB
					ClientDB[i].socket = -1;
					ClientDB[i].isAuthenticated = false;
				} 
				//Echo back the message that came in 
				else { 
					//set the string terminating NULL byte on the end 
					//of the data read 
					buffer[valread] = '\0';
					if (buffer == ""){
						send(sd , "Select Valid INPUT" , strlen("Select Valid INPUT") , 0);
						break;
					}
					struct replyPacket packet;
					packet = methodSelector(buffer, i, ClientDB);
					saveClientDB(ClientDB);

					//if need to know what server got:
					//printClientDB(ClientDB);
					//printPacket(packet);


					bridge(sd, client_socket, packet, ClientDB);
				} 
			} 
		} 
	} 
	return 0; 
} 
