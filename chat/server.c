#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <sys/socket.h> 
#include <sys/types.h> 


void chat(int client_fd) {
	char buff[50];
	int n;
	while(1) {
		bzero(buff, sizeof(buff));
		read(client_fd, buff, sizeof(buff));
		printf("Client: %s", buff);
		printf("reply: ");
		bzero(buff, sizeof(buff));
		n = 0;
		while((buff[n++] = getchar()) != '\n');

		write(client_fd, buff, sizeof(buff));
		if(strncmp(buff, "exit", 4) == 0) break;
	}
	printf("Server exit\n");

}

int create_lstn_sock_fd() {
	int lstn_sock_fd, flag;
	
	// struct sockaddr_in is defind inside lib (but you can also create the structure with same name and same data types inside new structure it will work.)
	struct sockaddr_in lstn_socket; 
	
	// AF_INET is address family name for IPv4 type addresses and SOCK_STREAM is for TCP and SOCK_DGRAM is for UDP.
	// in protocol families (eg. AF_INET) generally there is only one protocol like IPv4 hence protocol is 0.
	// socket(int domain, int type, int protocol);
	lstn_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(lstn_sock_fd == -1) {
		printf("listening socket creation failed\n");
		exit(0);
	} else printf("listening socket created\n");


	// socket is created now prepare the socket address structure
	// specify the protocol for socket address
	lstn_socket.sin_family = AF_INET;
	// htonl is actually to take care of little endian and big endian in i386 little endian is used but in network big endian is used. it convert host byte order to network byte order.
	// IP address is of machine not of any server running on machine right? so we cannot fix the IP address of my laptop from this piece of code so
	// INADDR_ANY is actaully 0.0.0.0 this means connect from all active ports and don't care about ip-address, means this is actually IP address of client to connect with but we want any client not specific.
	lstn_socket.sin_addr.s_addr = htonl(INADDR_ANY);
	// port no of client to connect with.
	lstn_socket.sin_port = htons(8080);

	// bind assign the address to socket third arg is address length in bytes.
	flag = bind(lstn_sock_fd, (struct sockaddr *)&lstn_socket, sizeof(lstn_socket));
	if(flag == -1) {
		printf("Bind failed\n");
		exit(0);
	} else printf("Bind successful\n");

	// make active socket to passive socket by listen() method. 5 is backlogs means max length of queue in listening socket
	// if queue length is more than backlogs then client will receive connection refused error.
	flag = listen(lstn_sock_fd, 5);
	if(flag == -1) {
		printf("Error listening on socket\n");
		exit(0);
	} else printf("listening...\n");

	return lstn_sock_fd;
}

int main() {
	int lstn_sock_fd, clnt_sock_fd, len;
	
	struct sockaddr_in client_addr;// when new connection is established then clients ip port etc. will be stored in client_addr;

	lstn_sock_fd = create_lstn_sock_fd();

	len = sizeof(client_addr);
	// sockaddr and sockaddr_in both are different.
	// accept the incoming connection request and create the socket for accepted connection and return the socket fd to communicate.
	clnt_sock_fd = accept(lstn_sock_fd, (struct sockaddr *)&client_addr, &len);
	if(clnt_sock_fd == -1) {
		printf("Error accepting: error:%d\n", clnt_sock_fd);
		exit(0);
	} else printf("connection accepted\n");

	chat(clnt_sock_fd);
	close(lstn_sock_fd);
}
