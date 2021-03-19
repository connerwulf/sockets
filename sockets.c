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
#define PORT 1050
#define SERVER_IP "131.247.3.8"
#define BUFFERSIZE 15

//Shared memory
char sharedArray[BUFFERSIZE];
sem_t semaphore;



//Thread function to save message to sharedArray
void * t_socket(void *arg)
{
  int sock = *((int *)arg);
  char temp[BUFFERSIZE];


  sem_wait(&semaphore);
    /* Critical Section Starts*/
    //Receieve message save to sharedArray and copy to temp array
    recv(sock, sharedArray, BUFFERSIZE, 0);
    strncpy(temp, sharedArray, BUFFERSIZE);
      /* Critical Section Ends*/
  sem_post(&semaphore);

  sleep(2);

  //send message back to client after 2 secs
  send(sock, temp, BUFFERSIZE, 0);
  close(sock);
  return(NULL);
}






int main(int argc, char const *argv[]) {

  sem_init(&semaphore,0,1);
  int server;
  struct sockaddr_in serverInfo;
  struct sockaddr_storage storage;


  //Server creates endpoint for communication with client
  if((server = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("Socket function failed.\n");
    exit(1);
  }

  //PROVIDED CODE
  /* This makes sure the port is immediately released when this code is done. */
  int on = 1;


/* This should free the socket right away */
  if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0)
  {
    perror("Stream Socket option set failed.\n");
    exit(1);
            /* some error has occurred, etc */
  }
//END OF PROVIDED CODE
  //Populated needed information to communicate over internet addressess
  serverInfo.sin_family = AF_INET;
  serverInfo.sin_port = htons(PORT);
  serverInfo.sin_addr.s_addr = inet_addr(SERVER_IP);

  //Assigning the address specific to the server socket
  if(bind(server, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) < 0)
  {
    perror("Binding of the socket failed\n");
    exit(1);
  }

  //Set bound for number of connections that can be queue for our socket
  if(listen(server, 3)< 0)
  {
      perror("Listening Failed\n");
      exit(1);
  }
  printf("listening...\n");


  int maxCon = 3;
  int curCon = 0;
  int acceptedSocket;
  socklen_t addressSize;
  pthread_t sThread[3];

  //Loop through n connections, where n is 3
  while(curCon < maxCon)
  {
      addressSize = sizeof(storage);
      printf("accpeting...\n");
      //Waits and accepts first pending connection from the listening socket
      acceptedSocket = accept(server, (struct sockaddr *) &storage, &addressSize);
      printf("accpeted.\n");

      //Create thread to save message and reply to client
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
  close(server);
  printf("\nServer Program Finished\n");
  exit(0);

}
