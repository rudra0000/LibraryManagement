#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
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
void deleteBook(char* bookname,char* username){
    int file=open("books.txt",O_RDONLY);
    if(file==-1){
        printf("Error opening books file\n");
        exit(EXIT_FAILURE);
    }
    ssize_t bytes_read;
    char buffer[1000];
    char line[1000];
    int line_length = 0;
    int line_no_of_book=-1;
    int current_line=0;
    printf("atleast we can be here\n");
    while ((bytes_read = read(file, buffer, 1000)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                line[line_length] = '\0';
                printf("%s line we got ff ff\n",line);
                char *saved_bookname = strtok(line, ":");
                char *author= strtok(NULL, ":");
                if (saved_bookname!=NULL && strcmp(saved_bookname,bookname) == 0) {
                    //found the book    
                    line_no_of_book=current_line;
                    break;
                }
                // Reset line buffer
                current_line++;
                line_length = 0;
            } else {
                // Copy character to line buffer
                line[line_length] = buffer[i];
                line_length++;
            }
        }
        if(line_no_of_book!=-1) break;
    }
    printf("book found on line number %d\n",line_no_of_book);
    if(line_no_of_book==-1){
        printf("Book not found\n");
        close(file);
        exit(EXIT_FAILURE);
    }
    printf("yo control reached here\n");
    lseek(file,0,SEEK_SET);
    //creating a new temporary file
    int new_file=open("temp.txt", O_WRONLY|O_CREAT|O_TRUNC,0644);
    if(new_file==-1){
        printf("Error opening new file\n");
        exit(EXIT_FAILURE);
    }
    line_length=0;
    current_line=0;
    while ((bytes_read = read(file, buffer, 1000)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                line[line_length] = '\0';
                printf("%s line we got\n",line);
                if(current_line!=line_no_of_book){
                    write(new_file,line,line_length);
                    write(new_file,"\n",1);
                }
                line_length = 0;
                current_line++;
            } else {
                // Copy character to line buffer
                line[line_length] = buffer[i];
                line_length++;
            }
        }
    }
    close(file);
    close(new_file);
    remove("books.txt");
    rename("temp.txt","books.txt");
    int transaction_file=open("transactionLogs.txt",O_WRONLY | O_APPEND);
    if(transaction_file==-1){
        printf("Error opening transactions file\n");
        exit(EXIT_FAILURE);
    }
    char message[2000]="";
    strcat(message,username);
    strcat(message," deleted the book ");
    strcat(message,bookname);
    strcat(message," from the library\n");
    printf("%s this is the message to be written into the transactions file\n",message);
    write(transaction_file,message,strlen(message));
    printf("%s deleted the book %s from the library\n",username,bookname);
    close(transaction_file);
}