//to write server code here using multithreading
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<pthread.h>
#include<fcntl.h>
#include"addition.h"
#include"book.h"
#define PORT 1085
#define MAX_CLIENTS 10
#define RECORD_SIZE 900
void* handleClient(void* arg){
    printf("this function is also being called\n");
    int client_socket=*((int*)arg);
    //handle client requests
    int choice;
    while(recv(client_socket,&choice,sizeof(choice),0)>0){
        // printf("%d we got as choice\n",choice);
        if(choice==1){
            char buffer[2000];
            recv(client_socket,buffer,sizeof(buffer),0);
            printf("%s this is the buffer received\n");
            char *username = strtok(buffer, ":");
            char *password = strtok(NULL, ":");
            addUser(username,password);
            char message[2000]="";
            strcat(message,username);
            strcat(message," user has been added to library\n");
            if(send(client_socket,message,strlen(message),0)==-1){
                printf("Error sending this message\n");
                exit(EXIT_FAILURE);
            }
        }else if(choice==2){
            char buffer[2000];
            recv(client_socket,buffer,sizeof(buffer),0);
            printf("%s this is the buffer received\n");
            char *username = strtok(buffer, ":");
            char *password = strtok(NULL, ":");
            addLibrarian(username,password);
            char message[2000]="";
            strcat(message,username);
            strcat(message," librarian has been added to library\n");
            if(send(client_socket,message,strlen(message),0)==-1){
                printf("Error sending this message\n");
                exit(EXIT_FAILURE);
            }
        }else if(choice==4){
            char buffer[2000];
            recv(client_socket,buffer,sizeof(buffer),0);
            char* bookname=strtok(buffer,":");
            char* numCopies=strtok(NULL,":");
            addBook(bookname,numCopies);
            char message[2000]="";
            strcat(message,bookname);
            strcat(message," book has been added to library\n");
            if(send(client_socket,message,strlen(message),0)==-1){
                printf("Error sending this message\n");
                exit(EXIT_FAILURE);
            }
        }else if(choice==7){
            char buffer[2000];
            recv(client_socket,buffer,sizeof(buffer),0);
            char* bookname=strtok(buffer,":");
            char* username=strtok(NULL,":");
            printf("%s is the username\n",username);
            issueBook(bookname,username);
            char message[2000]="";
            strcat(message,bookname);
            strcat(message," book has been issued by ");
            strcat(message,username);
            strcat(message,"\n");
            if(send(client_socket,message,strlen(message),0)==-1){
                printf("Error sending this message\n");
                exit(EXIT_FAILURE);
            }
        }else if(choice==5){
            char buffer[2000];
            recv(client_socket,buffer,sizeof(buffer),0);
            printf("received %s for delete book choice\n",buffer);
            char* bookname=strtok(buffer,":");
            char* username=strtok(NULL,":");
            deleteBook(bookname,username);
            char message[2000]="";
            strcat(message,bookname);
            strcat(message," book has been deleted from the library by ");
            strcat(message,username);
            strcat(message,"\n");
            if(send(client_socket,message,strlen(message),0)==-1){
                printf("Error sending this message\n");
                exit(EXIT_FAILURE);
            }
        }else if(choice==8){
            char buffer[2000];
            recv(client_socket,buffer,sizeof(buffer),0);
            char* bookname=strtok(buffer,":");
            char* username=strtok(NULL,":");
            returnBook(bookname,username);
            char message[2000];
            strcat(message,bookname);
            strcat(message," book has been returned by the user ");
            strcat(message,username);
            strcat(message,"\n");
            if(send(client_socket,message,strlen(message),0)==-1){
                printf("Error sending this message\n");
                exit(EXIT_FAILURE);
            }
        }else if(choice==6){
            char buffer[2000];
            recv(client_socket,buffer,sizeof(buffer),0);
            char* oldName=strtok(buffer,":");
            char* newName=strtok(NULL,":");
            updateName(oldName,newName);
            char message[2000];
            strcat(message,oldName);
            strcat(message," updated to ");
            strcat(message,newName);
            strcat(message,"\n");
            if(send(client_socket,message,strlen(message),0)==-1){
                printf("Error sending this message\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    close(client_socket);
    pthread_exit(NULL);
}
int main(){
    int server_fd,new_socket;
    struct sockaddr_in address,client_addr;
    socklen_t addrlen=sizeof(address);
    pthread_t threads[MAX_CLIENTS];
    int thread_cnt=0;
    if((server_fd=socket(AF_INET,SOCK_STREAM,0))<0){
        printf("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);
    int val=bind(server_fd,(struct sockaddr*)&address,sizeof(address));
    printf("%d\n",val);
    if(val<0){
        perror("bind is failing\n");
        printf("bind failed\n");
        exit(EXIT_FAILURE);
    }
    // listen
    if(listen(server_fd,MAX_CLIENTS)==-1){
        printf("Listen failed\n");
        exit(EXIT_FAILURE);
    }
    while(1){
        int sz=sizeof(client_addr);
        new_socket=accept(server_fd,(struct sockaddr*)&client_addr,&sz);
        if(new_socket==-1){
            printf("Accept failed\n");
            continue;
        }else{
            printf("hey new connection estb\n");
        }
        if(pthread_create(&threads[thread_cnt],NULL,handleClient,&new_socket)!=0){
            printf("Thread creation failed\n");
            close(new_socket);
        }else{
            printf("hey new thread created to entertain a new request\n");
        }
        thread_cnt++;
    }
    close(server_fd);
    return 0;
}