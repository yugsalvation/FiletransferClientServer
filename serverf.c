#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


void serviceClient(int);

int main(int argc, char *argv[]){  
  int sd, client, portNumber, status;
  struct sockaddr_in servAdd;      // client socket address
  
 if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Cannot create socket\n");
    exit(1);
  }
  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);
  
  bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
  listen(sd, 5);

  while(1){
    client = accept(sd, NULL, NULL);
    printf("Got a client, start chatting\n");
    
    if(!fork())
      serviceClient(client);

    close(client);
    waitpid(0, &status, WNOHANG);
  }
}

void serviceClient(int sd){
	char message[255];
    char filename[255];
        int n, pid;
        int fd,n1;

        char r[] = "start chatting: You have 3 options get filename, put filename and quit";
        write(sd, r, sizeof(r));
    int client;
	pid=fork();
	if(pid){                 /* reading client messages */
       while(1){
            if(n=read(sd, message, 255)){
	        message[n]='\0';
            printf("Client/%d: %s", getpid(), message);
	        //fprintf(stderr,"%s", message);
	        char * token = strtok(message, " ");
            char reply[255],flag[100];
            int count=0;
            char ack[50];
            if(!strcasecmp(message, "quit\n")){
    	        kill(pid, SIGTERM);
                //kill(getppid(), SIGTERM);
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
                   
                    
                   fd = open(filename, O_RDONLY);
                    read(fd, reply, 100);
                      
                    
                      if(fd == -1){
                         strcpy(flag,"File doesn't exist\n"); 
                        write(1, flag, strlen(flag));
                        write(sd,"h",1);                        
                    }
                    else{
                        strcpy(flag,"File sent successfully\n"); 
                        write(1, flag, strlen(flag));
                        
                        write(sd, reply, strlen(reply));
                       // printf("dsdsd: %s\n",reply);
                     }
                    
            }
            else if(!strcasecmp(token, "put")){    
                    while (token != NULL)
                    {
                        if(count++==1){
                            strcpy(filename,token);
                            break;
                          }
                         token = strtok (NULL, " ");
                    }
                    filename[strlen(filename)-1] = '\0';
                    
                    fd = open(filename,O_CREAT|O_WRONLY,0777); 
                    n1=read(sd, reply, 255);
                    write(fd,reply,strlen(reply));
                     if(n1 <= 1){
                                printf("File doesn't exist at server side\n");
                            }
                     else{
                                printf("File downloaded\n");
                        }                     

            }
            else{
               strcpy(message,"invalid request from client");
               write(sd, message, strlen(message)+1);
            } 
            printf("\n");
	   }




	   /*if(n=read(sd, message, 255)){
	     message[n]='\0';
	     fprintf(stderr,"%s\n", message);
         //char * token = strtok(message, " ");
	     
         if(!strcasecmp(message, "Bye\0")){
	        kill(pid, SIGTERM);
	        exit(0);
	     }
         else if(!strcasecmp(message, "put\0")){
            printf("inside put");     
            //message[n]='\0';
	      write(sd, message, strlen(message)+1);
        }
        else{
             printf("invalid"); 
            //message[n]='\0';
	      write(sd, message, strlen(message)+1);
         } */
    }
}

//	if(!pid)                      /* sending messages to the client  */
/*	   while(1)
       
*/
}
