#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#define RECORD_SIZE 900
int addLibrarian(char* username, char* password){
    // Open file for appending
    int file = open("librarians.txt", O_WRONLY | O_APPEND);
    struct flock lock;
    lock.l_type=F_WRLCK; //mandatory locking
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=RECORD_SIZE;
    if(file == -1) {
        printf("Error opening librarians file\n");
        exit(EXIT_FAILURE);
    }
    //lock the file
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error locking librarians file\n");
        exit(EXIT_FAILURE);
    }
    // Format the string to write
    char buffer[RECORD_SIZE];
    snprintf(buffer, RECORD_SIZE, "%s:%s\n", username, password);
    printf(" to write %s\n",buffer);
    // Write to file
    int bytes_written = write(file, buffer, strlen(buffer));
    if (bytes_written == -1) {
        printf("Error writing to librarians\n");
        close(file);
        exit(EXIT_FAILURE);
    }
    //unlocking file
    lock.l_type=F_UNLCK;
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error unlocking Librarians file\n");
        exit(EXIT_FAILURE);
    }
    printf("hello we are here\n");
    printf("Librarian successfully added\n");
    close(file);
    return 1;
}
int addUser(char* username, char* password){
    // Open file for appending
    int file = open("users.txt", O_WRONLY | O_APPEND);
    struct flock lock;
    lock.l_type=F_WRLCK; //mandatory locking
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=RECORD_SIZE;
    if(file == -1) {
        printf("Error opening users file\n");
        exit(EXIT_FAILURE);
    }
    //lock the file
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error locking users file\n");
        exit(EXIT_FAILURE);
    }
    // Format the string to write
    char buffer[RECORD_SIZE];
    snprintf(buffer, RECORD_SIZE, "%s:%s\n", username, password);

    // Write to file
    int bytes_written = write(file, buffer, strlen(buffer));
    if (bytes_written == -1) {
        printf("Error writing to users\n");
        close(file);
        exit(EXIT_FAILURE);
    }
    //unlocking the file
    lock.l_type=F_UNLCK;
    if(fcntl(file,F_SETLKW,&lock)==-1){
        printf("Error unlocking Users file\n");
        exit(EXIT_FAILURE);
    }
    printf("User successfully added\n");
    close(file);
    return 1;
}