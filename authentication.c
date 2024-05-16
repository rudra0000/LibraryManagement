#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#define RECORD_SIZE 900
void addLibrarian(char* name){
    struct flock lock;
    lock.l_type=F_WRLCK; //mandatory locking
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=RECORD_SIZE;
    strcat(name,"\n");
    int file=open("librarians.txt",O_WRONLY | O_APPEND);
    if(file==-1){
        printf("Error opening librarians file\n");
        exit(1);
    }
    //lock the file
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error locking librarians file\n");
        exit(EXIT_FAILURE);
    }
    int sz=write(file,name,strlen(name));
    printf("Librarian successfully added\n");
    lock.l_type=F_UNLCK;
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error unlocking librarians file\n");
        exit(EXIT_FAILURE);
    }
    close(file);
}
void addUser(char *name){
    struct flock lock;
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=RECORD_SIZE; //mandatory locking
    strcat(name,"\n");
    int file=open("users.txt",O_WRONLY | O_APPEND);
    if(file==-1){
        printf("Error opening users file\n");
        exit(1);
    }
    //lock the file
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error locking users file\n");
        exit(EXIT_FAILURE);
    }
    int sz=write(file,name,strlen(name));
    printf("User successfully added\n");
    lock.l_type=F_UNLCK;
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error unlocking users file\n");
        exit(EXIT_FAILURE);
    }
    close(file);
}
int isLibrarianPresent(char* name){
    int file;
    ssize_t bytes_read;
    char buffer[1000];
    char line[1000];
    int line_length = 0;
    //initialize lock
    struct flock lock;
    lock.l_type=F_RDLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0; //adviosry locking 
    lock.l_len=RECORD_SIZE;
    file = open("librarians.txt", O_RDONLY);
    if (file == -1) {
        printf("Error opening file");
        exit(EXIT_FAILURE);
    }
    //lock the file now
    if(fcntl(file,F_SETLK,&lock)==-1){
        printf("Error locking Librarians file\n");
        exit(EXIT_FAILURE);
    }
    while ((bytes_read = read(file, buffer, 1000)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                line[line_length] = '\0'; 
                if(!strcmp(line,name)){
                    printf("Librarian found successfully\n");
                    close(file);
                    return 1;
                }
                write(STDOUT_FILENO, "\n", 1);
                line_length = 0;
            } else {
                line[line_length] = buffer[i];
                line_length++;
            }
        }
    }
    //unlocking the file
    lock.l_type=F_UNLCK;
    if(fcntl(file,F_SETLK,&lock)==-1){
        printf("Error unlocking Librarians file\n");
        exit(EXIT_FAILURE);
    }
    close(file);
    return 0;
}
int isUserPresent(char* name){
    int file;
    ssize_t bytes_read;
    char buffer[1000];
    char line[1000];
    int line_length = 0; // advisory locking 
    //initialize lock
    struct flock lock;
    lock.l_type=F_RDLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=RECORD_SIZE;
    file = open("users.txt", O_RDONLY);
    if (file == -1) {
        printf("Error opening file");
        exit(EXIT_FAILURE);
    }
    //lock the file now
    if(fcntl(file,F_SETLK,&lock)==-1){
        printf("Error locking Librarians file\n");
        exit(EXIT_FAILURE);
    }
    while ((bytes_read = read(file, buffer, 1000)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                line[line_length] = '\0'; 
                if(!strcmp(line,name)){
                    printf("User found successfully\n");
                    close(file);
                    return 1;
                }
                write(STDOUT_FILENO, "\n", 1);
                line_length = 0;
            } else {
                line[line_length] = buffer[i];
                line_length++;
            }
        }
    }
    //unlocking the file
    lock.l_type=F_UNLCK;
    if(fcntl(file,F_SETLK,&lock)==-1){
        printf("Error unlocking Librarians file\n");
        exit(EXIT_FAILURE);
    }
    close(file);
    return 0;
}
int main(){
    char name[]="tom silverwood";
    // printf("%d\n",isUserPresent(name));
    addUser(name);
    return 0;
}