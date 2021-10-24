#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include <time.h>
#include <sys/select.h>

#define PORT 5555
#define MAXSIZEINPUT 550

int main(int argc, char const *argv[]) { 
	int sock = 1, valread; 
	struct sockaddr_in serv_addr; 
	char input[MAXSIZEINPUT+1];
	char condition[5];
	char buffer[1024]; 

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) { 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
		printf("\nConnection Failed \n"); 
		return -1; 
	}

	fd_set write_fds;
	
	memset(buffer, 0, sizeof(buffer));
	buffer[0] = '\0';

	while(1){
		scanf("%[^\n]%*c", input);
		if(strcmp(input, "exit") == 0)
			break;
		if(strcmp(input, "update") != 0){
			if (send(sock, input, strlen(input), 0) == -1)
				break;
			printf("Input enviado\n");
		}
		

		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
		
		while ((valread = read(sock , buffer, 1024)) > 0){
			printf("%s", buffer);
			memset(buffer, 0, sizeof(buffer));
			buffer[0] = '\0';
		}
		printf("nothing new...\n\n");
	}

	return 0; 
} 
