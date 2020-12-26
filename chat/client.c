#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <sys/socket.h> 
#include <sys/types.h> 

void chat(int server_fd) {
	char buff[50];
	int n;
	while(1) {
		bzero(buff, sizeof(buff));
		n = 0;
		while((buff[n++] = getchar()) != '\n');

		write(server_fd, buff, sizeof(buff));
		if(strncmp(buff, "exit", 4) == 0) break;

		bzero(buff, sizeof(buff));
		read(server_fd, buff, sizeof(buff));
		printf("Server: %s", buff);
		printf("reply: ");
	}
	printf("Client exit\n");
}


void main() {
	struct sockaddr_in server_address;

	int sock_fd, flag;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd == -1) {
		printf("socket creation failed");
		exit(0);
	} else printf("socket created\n");

	server_address.sin_family = AF_INET;
	// IP of server pc to connect with. INADDR_LOOPBACK is 127.0.0.1 i.e. localhost you can specify IP
	server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);//inet_addr("127.0.0.1");
	// port of server process on server pc.
	server_address.sin_port = htons(8080);

	// sockeaddr is superset of sockaddr_in (ipv4), sockaddr_in6 (ipv6), sockaddr_un (linux local address) 
	//hence in connect method each type will be checked again. this is bad design pattern in library.
	flag = connect(sock_fd, (struct sockaddr *)&server_address, sizeof(server_address));
	if(flag == -1) {
		printf("Error conecting\n");
		exit(0);
	} else printf("connected\n");

	chat(sock_fd);
}