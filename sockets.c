#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

//PORT AND IP CONGIFURATION
//#define PORT 1050 //FOR TURN IN
#define PORT 1077
#define SERVER_IP 131.247.3.8

char sharedArray[16];

int int main(int argc, char const *argv[]) {

  int clientServer;
  struct sockaddr_in serverInfo;

  if(clientServer = socket(AF_INET, SOCK_STREAM) == 0)
  {
    perror("Socket function failed.");
    exit(1);
  }

  /* This makes sure the port is immediately released when this code is done. */
  int on = 1;

/* This should free the socket right away */
  if (setsockopt(clientServer, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0)
  {
    perror("Stream Socket option set failed.");
    exit(1);
            /* some error has occurred, etc */
  }

  serverInfo.sin_family = AF_INET;
  serverInfo.sin_port = htons(PORT);
  serverInfo.sin_addr = inet_addr(SERVER_IP);

  if(bind(clientServer, (struct sockaddr *)&serverInfo, sizeof(address)) < 0)
  {
    perror("Binding of the socket failed");
    exit(1);
  }

  if(listen(clientServer, 3))< 0)
    {
        perror("Listening Failed");
        exit(1);   
    }

}
