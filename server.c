//server Code:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct Packet {
    int seq;
    char data[10];
};

int count = 1;
struct Packet ack;
int prev=0;
int dupAcks=0;
int ssthresh = 4;
int time=0;
static int flag=1;

void processBinaryData(const char *filename, struct Packet packets[], int size) {
    FILE *file = fopen(filename, "rb"); // Open the file in binary mode

    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int packetSize = 4;
    int bytesRead;
    int packetIndex = 0;

    while ((bytesRead = fread(packets[packetIndex].data, 1, 4, file)) == 4) {
        packets[packetIndex].data[4] = '\0';
        packets[packetIndex].seq = packetIndex;
        packetIndex++;

        // Check if we have reached the end of the array
        if (packetIndex == size)
            break;
    }

    fclose(file);
}

void sendPacket(int *cwnd, struct Packet *packets, int *size, int client, int status, FILE *data) {
      
    if (status == 1 && dupAcks !=3 && flag ) {
        write(client, &packets[*size-1], sizeof(struct Packet));
        flag=1;
        printf("\ncwnd: %d/%d\n", count++, *cwnd);
     
        printf("Packet Sent   : %d", packets[*size-1].seq );
        (*size)--;
     
        read(client, &ack, sizeof(ack));
        printf("%s - %d",ack.data,ack.seq);
    
        
        if(ack.seq == prev)
        {
          dupAcks++;
          printf("  Duplicate ACKs : %d",dupAcks);
     
        }
        else dupAcks=0;
          
        prev = ack.seq;
        
    
      }
    else if(status ==0)
    {
       // printf("old size : %d",*size);
       (*size)--; //printf("new size : %d",*size);
    }
    if(dupAcks==3 && flag==0)
      {
          ssthresh = *cwnd/2;
          *cwnd=1; 
          dupAcks = 0;
          count=1;
          
          write(client, &packets[ack.seq-1], sizeof(struct Packet));
          printf("Retransmitting packet : %d", packets[ack.seq-1].seq );
          flag=1;
          return;
          //*size = ack.seq - 1;
        
      }
    else if((count - 1) == *cwnd)
    {
          fprintf(data, "%d %d\n",time++,*cwnd);
          if(*cwnd >= ssthresh )
          {
               (*cwnd)++;
               count=1;
             
          }
          else
          {
                count=1;
                (*cwnd) *=2;
          }
    }

    if(dupAcks !=3) flag=1;
    else flag=0;

    
}


int main(int argc, char **argv) {

    int size = 30;
    FILE *data = fopen("output.xg","w");
    fprintf(data, "color = blue\n");
    fprintf(data, "thickness = 2.5\n");
    fprintf(data, "title_x = Number of Transmissions \n");
    fprintf(data, "title_y = Congestion Window Size\n");
    
    
    struct Packet* packets = malloc(sizeof(struct Packet)*size);

    processBinaryData("input.txt", packets, size);
    
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[1]));
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 1) == -1) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    printf("\nWaiting for connections...\n");

    int clientSocket;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    if (clientSocket == -1) {
        perror("Connection Error");
        exit(EXIT_FAILURE);
    }

    printf("Connected to client: %s\n", inet_ntoa(clientAddress.sin_addr));

    int cwnd = 1, ssthresh;
    struct Packet recvd;
    int status;
    int flag = 0;
    
    printf("---------------");
    printf("\n Seq_No\t: Data\n");
    printf("---------------");
    
    for (int i = 0; i < (size); i++) {
        char intnum[30];
        printf("\n %d\t: %s",packets[i].seq, packets[i].data);
    } 
    
    while (1) {
        printf("\nSend Packet ? : ");
        scanf("%d", &status);
        sendPacket(&cwnd, packets, &size, clientSocket, status,data);
        
        if(size < 0) 
        {
            struct Packet EOC;
            strcat(EOC.data," EOF");
            ack.data[0] = ' ';
            EOC.seq= -999;
            
            write(clientSocket, &EOC, sizeof(struct Packet));
            printf("\n\n");
        break;
        
        } 
    }
    
    close(serverSocket);
    return 0;
}
