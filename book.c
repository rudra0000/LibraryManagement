#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#define RECORD_SIZE 900
void addBook(char * bookname,char* author){
    int file=open("books.txt",O_WRONLY | O_APPEND);
    struct flock lock;
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=RECORD_SIZE;
    if(file==-1){
        printf("Error opening books file\n");
        exit(EXIT_FAILURE);
    }
    //locking file
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error locking books file\n");
        exit(EXIT_FAILURE);
    }
    char buffer[RECORD_SIZE];
    snprintf(buffer,RECORD_SIZE,"%s:%s\n",bookname,author);
    //writing into the file
    int bytes_written=write(file,buffer,strlen(buffer));
    if(bytes_written==-1){
        printf("Error writing into books file\n");
        close(file);
        exit(EXIT_FAILURE);
    }
    //unlocking file
    lock.l_type=F_UNLCK;
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error unlocking books file\n");
        exit(EXIT_FAILURE);
    }
    printf("Book added to library successfully\n");
    close(file);
}
void issueBook(char* bookname,char* username){
    //todo
    //add the transaction to transactionLogs.txt
    char buffer[RECORD_SIZE];
    snprintf(buffer,RECORD_SIZE,"%s:%s:issue\n",bookname,username);
    int file=open("transactionLogs.txt",O_WRONLY|O_APPEND);
    if(file==-1){
        printf("Error opening transactions file\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=RECORD_SIZE;
    //locking file
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error locking transactions file\n");
        exit(EXIT_FAILURE);
    }
    int bytes_written=write(file,buffer,strlen(buffer));
    if(bytes_written==-1){
        printf("Error writing into transactions file\n");
        close(file);
        exit(EXIT_FAILURE);
    }
    //unlocking file
    lock.l_type=F_UNLCK;
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error unlocking transactions file\n");
        exit(EXIT_FAILURE);
    }
    printf("%s issued book %s from library\n",username,bookname);
    close(file);
}
void returnBook(char* bookname,char* username){
    //todo
    //add the transaction to transactionLogs.txt
    char buffer[RECORD_SIZE];
    snprintf(buffer,RECORD_SIZE,"%s:%s:return\n",bookname,username);
    int file=open("transactionLogs.txt",O_WRONLY|O_APPEND);
    if(file==-1){
        printf("Error opening transactions file\n");
        exit(EXIT_FAILURE);
    }
    struct flock lock;
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=RECORD_SIZE;
    //locking file
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error locking transactions file\n");
        exit(EXIT_FAILURE);
    }
    int bytes_written=write(file,buffer,strlen(buffer));
    if(bytes_written==-1){
        printf("Error writing into transactions file\n");
        close(file);
        exit(EXIT_FAILURE);
    }
    //unlocking file
    lock.l_type=F_UNLCK;
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error unlocking transactions file\n");
        exit(EXIT_FAILURE);
    }
    printf("%s returned book %s from library\n",username,bookname);
    close(file);
}
int isBookPresent(char* bookname){
    //todo
}
void viewTransactionHistory(char* username){
    //todo
}