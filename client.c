//Client Code:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct Packet {
    int seq;
    char data[10];
};

int checkDUPS(struct Packet *buf, int seq, int pointer)
{
    for(int i=0; i<pointer; i++)
    {
      if(buf[i].seq == seq)
      {
          return 1;
      }
    
    }
    
    return 0;

}

struct Packet RECEIVED_BUFFER[100];
struct Packet *recvd;

void recvPackets(int client) {
    int clientSocket = client;
    struct Packet ack;
    strcat(ack.data,"ACK");
    ack.data[0] = ' ';


    while (1) {
        int bytes = read(clientSocket, recvd, sizeof(struct Packet));
        static int recent = 29;
        static int pointer = 0;

       
                
        if (bytes > 0) {
        
        if(recvd->seq==-999)
        {
            printf("\n%s",recvd->data);
            break;
        }
        int flag = checkDUPS(RECEIVED_BUFFER, recvd->seq, pointer);
        if(flag) continue;
        
            if(recvd->seq == recent && flag==0)
            {
               
                
                ack.seq = recvd->seq;
                if(ack.seq != recent)
                {
                    ack.seq = recvd->seq - 3;
                }
  
                
                
                printf("\n Packet Received : %s -> %d", recvd->data, recvd->seq);
              
                RECEIVED_BUFFER[pointer++] = *recvd;
                 recent--;
                write(clientSocket, &ack, sizeof(ack));
            }
            else
            {
                //ack.seq = recvd->seq + 5;
                  RECEIVED_BUFFER[pointer++] = *recvd;
                  recent--;
                  printf("\n Packet Received : %s -> %d", recvd->data, recvd->seq);
                  write(clientSocket, &ack, sizeof(ack));
            }
            
            
            

        }
    }
}

int main(int argc, char **argv) {
    int size = 20;

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[1]));
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change to the server's IP address

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Connection Error");
        exit(EXIT_FAILURE);
    }

    printf("\nConnected to the server\n");

    recvd = malloc(sizeof(struct Packet));
    recvPackets(clientSocket);
    free(recvd);
    close(clientSocket);

    return 0;
}
