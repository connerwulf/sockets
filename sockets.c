#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

//PORT AND IP CONGIFURATION
//#define PORT 1050 //FOR TURN IN
#define PORT 1077
#define SERVER_IP "131.247.3.8"
#define BUFFERSIZE 16
char sharedArray[BUFFERSIZE];
sem_t semaphore;




void * t_socket(void *arg)
{
  int sock = *((int *)arg);
  char temp[BUFFERSIZE];
  recv(sock, temp, BUFFERSIZE, 0);

  sem_wait(&semaphore);
    /* Critical Section */
    strncpy(sharedArray, temp, BUFFERSIZE);
    sleep(2);
    send(sock, sharedArray, BUFFERSIZE, 0);
  sem_post(&semaphore);

  close(sock);
  printf("\nThread is done\n");
  return(NULL);
}






int main(int argc, char const *argv[]) {

  sem_init(&semaphore,0,1);
  int clientServer;
  struct sockaddr_in serverInfo;
  struct sockaddr_storage storage;
  socklen_t addressSize;


  if(clientServer = socket(AF_INET, SOCK_STREAM, 0) == 0)
  {
    perror("Socket function failed.\n");
    exit(1);
  }

  /* This makes sure the port is immediately released when this code is done. */
  int on = 1;

/* This should free the socket right away */
  if (setsockopt(clientServer, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0)
  {
    perror("Stream Socket option set failed.\n");
    exit(1);
            /* some error has occurred, etc */
  }

  serverInfo.sin_family = AF_INET;
  serverInfo.sin_port = htons(PORT);
  serverInfo.sin_addr.s_addr = inet_addr(SERVER_IP);

  if(bind(clientServer, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) < 0)
  {
    perror("Binding of the socket failed\n");
    exit(1);
  }

  if(listen(clientServer, 3)< 0)
  {
      perror("Listening Failed\n");
      exit(1);
  }

  int maxCon = 3;
  int curCon = 0;
  int acceptedSocket;
  pthread_t sThread[3];
  while(curCon < maxCon)
  {
      addressSize = sizeof storage;
      acceptedSocket = accept(clientServer, (struct sockaddr *) &storage, &addressSize);

      if(pthread_create(&sThread[curCon], NULL, t_socket, &acceptedSocket) != 0)
      {
        printf("Failed trying to create a thread\n");
        exit(1);
      }
      curCon++;
  }
  for(int g = 0; g < maxCon; g++)
  {
    pthread_join(sThread[g], NULL);
  }

  printf("\nServer Program Finished\n");
  exit(0);

}
