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
#define PORT 1079
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
    if(choice==1){
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
        printf("hello we can be here\n");
        printf("choice is %d\n",choice);
        if(flag==2){
            printf("1.add user\n");
            printf("2.add librarian\n");
            printf("3. logout\n");
            printf("4.add book\n");
            scanf("%d",&choice);
        }else if(flag==1){
            //write menu for user
            printf("5. issue book\n");
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
                // send(client_socket,password,sizeof(password),0);
            }else if(choice==3){
                printf("bye\n");
                break;
            }else if(choice==4){
                char bookname[2000], cnt[2000];
                scanf("%s",bookname);
                scanf("%s",&cnt);
                strcat(bookname,":");
                strcat(bookname,cnt);
                send(client_socket,bookname,sizeof(bookname),0);
            }else if(choice==5){
                char bookname[2000],username[2000];
                scanf("%s",bookname);
                scanf("%s",username);
                strcat(bookname,":");
                strcat(bookname,username);
                send(client_socket,bookname,sizeof(bookname),0);
            }
        }
    }
    close(client_socket);
    return 0;
}
