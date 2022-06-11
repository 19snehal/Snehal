#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> 


sem_t x;
pthread_t t1[60];
char buf[20];
int sfd, cfd, port_no;
//pthread_t readerthreads[50];
int i=0;


void *routine1(void *msg)
{
    sem_wait(&x);
    printf("Hello from client 1\n");
	int cfd = *((int *)msg);
    //readerthreads ++;


	if (send(cfd, buf, strlen(buf), 0) < 0)
	{
		perror("send");
		exit(7);
	}
	close(cfd);
	close(sfd);
    sem_post(&x);
    pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: %s <port_no>\n", argv[0]);
		exit(1);
	}

	
	port_no = strtoul(argv[1], NULL, 10);

		/*
	Create your Socket do error checking
	Remember socket returns a socket descriptor
	SOCK_STREAM --->TCP
	or
	SOCK_DGRAM  --->UDP
	AF_INET ------->protocol/address family

	*/
	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(2);
	}

	struct sockaddr_in saddr = {0};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port_no);
	saddr.sin_addr.s_addr = INADDR_ANY;// Accept any ip address
	//memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
	
	sem_init(&x, 0, 1);
	//1. Bind is used for assigning port
	if (bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) 
	{
		perror("bind");
		close(sfd);
		exit(3);
	}
	
	//2. waits for incoming connection
	if (listen(sfd, 5) < 0) 
	{
		perror("listen");
		close(sfd);
		exit(4);
	}

	while(1)
	{
		//3. Accepts the incoming connection

		struct sockaddr_in caddr = {0};
		socklen_t len = sizeof(caddr);
		if ((cfd = accept(sfd, (struct sockaddr *)&caddr, &len)) < 0) 
		{
			perror("accept");
			exit(5);
		}
	
		// Creater readers thread
		if (pthread_create(&t1[i++],NULL,routine1, &cfd)!= 0)
		{
			printf("Failed to create thread\n");
		} 
		
		if (i >= 10) {
				// Update i
		i = 0;
			
	
		while (i < 10) {
					// Suspend execution of
					// the calling thread
					// until the target
					// thread terminates
		pthread_join(t1[i++],NULL);
					
					
				}
	
			}    // Update i
				i = 0;
	
	// To make a program like ECHO
		
	}
		return 0;
		sem_destroy(NULL);
}
