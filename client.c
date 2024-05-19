//estabilish connection with the server
//authenticate user/librarian
//make requests to the server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include"authentication.h"
#define PORT 1085
int main(){
    int client_socket;
    int choice;
    struct sockaddr_in server_addr;
    char buffer[1024]={0};
    int flag;
    printf("1.Login as user\n");
    printf("2.Login as librarian\n");
    scanf("%d",&flag);
    char username[1000],password[1000];
    scanf("%s",username);
    scanf("%s",password);
    int verified=0;
    if(flag==1){
        verified=authenticateUser(username,password);
    }else{
        verified=authenticateLibrarian(username,password);
    }
    if(!verified){
        printf("Invalid input\n");
        exit(EXIT_FAILURE);
    }
    //create socket
    if((client_socket=socket(AF_INET,SOCK_STREAM,0))==-1){
        printf("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(PORT);
    //connect to server
    if(connect(client_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1){
        printf("Connection failed\n");
        exit(EXIT_FAILURE);
    }
    while(1){
        //display menu to the user
        if(flag==2){
            printf("1.add user\n");
            printf("2.add librarian\n");
            printf("3.logout\n");
            printf("4.add book\n");
            printf("5.delete book\n");
            printf("6. update book name\n");
            scanf("%d",&choice);
        }else if(flag==1){
            //write menu for user
            printf("7. issue book\n");
            printf("8. return book\n");
            printf("9. logout\n");
            scanf("%d",&choice);
        }
        if(send(client_socket,&choice,sizeof(choice),0)==-1){
            printf("Send failed\n");
            exit(EXIT_FAILURE);
        }else{
            if(choice==2 || choice==1){
                char username[2000],password[1000];
                scanf("%s",username);
                scanf("%s",password);
                strcat(username,":");
                strcat(username,password);
                send(client_socket,username,sizeof(username),0);
            }else if(choice==3 || choice==9){
                printf("bye\n");
                break;
            }else if(choice==4){
                char bookname[2000], author[2000];
                scanf("%s",bookname);
                scanf("%s",&author);
                strcat(bookname,":");
                strcat(bookname,author);
                send(client_socket,bookname,sizeof(bookname),0);
            }else if(choice==5 || choice==7 || choice==8){
                char bookname[2000],username[2000];
                scanf("%s",bookname);
                scanf("%s",username);
                printf("%s\n",username);
                strcat(bookname,":");
                strcat(bookname,username);
                send(client_socket,bookname,sizeof(bookname),0);
            }
            else if(choice==6){
                char newName[2000],oldName[2000];
                scanf("%s",oldName);
                scanf("%s",newName);
                strcat(oldName,":");
                strcat(oldName,newName);
                send(client_socket,oldName,sizeof(oldName),0);
            }
            char message[2000]="";
            recv(client_socket,message,sizeof(message),0);
            printf("%s",message);
        }
    }
    close(client_socket);
    return 0;
}
