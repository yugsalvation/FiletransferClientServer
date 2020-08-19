#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[]){
  char message[255];
  char filename[255];
  int server, portNumber, pid, n;
  struct sockaddr_in servAdd;     // server socket address
  int fd,n1;
  
 if(argc != 3){
    printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
  }

  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
     fprintf(stderr, "Cannot create socket\n");
     exit(1);
  }

  servAdd.sin_family = AF_INET;
  sscanf(argv[2], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);

  if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
  fprintf(stderr, " inet_pton() has failed\n");
  exit(2);
}

 if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
  fprintf(stderr, "connect() has failed, exiting\n");
  exit(3);
 }

  read(server, message, 255);
  fprintf(stderr, "message received: %s\n", message);

  pid=fork();

  
  

  if(pid)                           /* sending messages to server */
     while(1)
      if(n=read(0, message, 255)){

         message[n]='\0';        
         write(server, message, strlen(message)+1);
        char * token = strtok(message, " ");
        int count=0;
        char flag[100],reply[255],ack[50];
         if(!strcasecmp(message, "quit\n")){
            kill(getppid(), SIGTERM);
            exit(0);
          }
         else if(!strcasecmp(token, "get")){
                        
                   while (token != NULL)
                    {
                        if(count++==1)
                            strcpy(filename,token);
                         token = strtok (NULL, " ");
                    }
                    filename[strlen(filename)-1] = '\0';
                    
                    fd = open(filename,O_CREAT|O_WRONLY,0777); 
                    
                    n1=read(server, reply, 255);
                     write(fd,reply,strlen(reply));
                
                           
                            if(n1 <= 1){
                                printf("File doesn't exist\n");
                            }
                             else{

                                printf("File downloaded\n");
                            }
                      
               }
          else if(!strcasecmp(token, "put")){
                   while (token != NULL)
                    {
                        if(count++==1)
                            strcpy(filename,token);
                         token = strtok (NULL, " ");
                    }
                    filename[strlen(filename)-1] = '\0';


                    fd = open(filename, O_RDONLY);
                    n1=read(fd, reply, 255);

                     if(fd == -1){
                         strcpy(flag,"File doesn't exist at client side\n"); 
                        write(1, flag, strlen(flag));
                        write(server,"h",1);                        
                    }
                    else{
                        strcpy(flag,"File sent successfully\n"); 
                        write(1, flag, strlen(flag));
                        
                        write(server, reply, strlen(reply));
                        //printf("dsdsd: %s\n",reply);
                     }

                   
          }
          else {
                strcpy(flag,"Invalid request. Try again\n"); 
                   write(1, flag, strlen(flag));
            }
        printf("\n");
        }    
}
