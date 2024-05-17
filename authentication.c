#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include"addition.h"
#define RECORD_SIZE 900
int authenticateLibrarian(char* username, char* password) {
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
    // Open the file for reading
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
    // Read the file line by line
    while ((bytes_read = read(file, buffer, 1000)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                // End of line reached, null-terminate the string
                line[line_length] = '\0';
                printf("%s line we got\n",line);
                // Extract username and password from the line
                char *saved_username = strtok(line, ":");
                char *saved_password = strtok(NULL, ":");
                printf("%s %s\n",saved_username,saved_password); /////////////////////////////////////////////
                // Check if the username and password match
                if (strcmp(saved_username, username) == 0 && strcmp(saved_password, password) == 0) {
                    printf("Authentication successful\n");
                    //unlocking the file
                    lock.l_type=F_UNLCK;
                    if(fcntl(file,F_SETLK,&lock)==-1){
                        printf("Error unlocking Librarians file\n");
                        exit(EXIT_FAILURE);
                    }
                    close(file);
                    return 1; // Librarian found
                }

                // Reset line buffer
                line_length = 0;
            } else {
                // Copy character to line buffer
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
    // Close the file
    close(file);

    // Authentication failed
    printf("Authentication failed\n");
    return 0;
}
int authenticateUser(char* username, char* password) {
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
    // Open the file for reading
    file = open("users.txt", O_RDONLY);
    if (file == -1) {
        printf("Error opening file");
        exit(EXIT_FAILURE);
    }
    //lock the file now
    if(fcntl(file,F_SETLK,&lock)==-1){
        printf("Error locking User file\n");
        exit(EXIT_FAILURE);
    }
    // Read the file line by line
    while ((bytes_read = read(file, buffer, 1000)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                // End of line reached, null-terminate the string
                line[line_length] = '\0';

                // Extract username and password from the line
                char *saved_username = strtok(line, ":");
                char *saved_password = strtok(NULL, ":");

                // Check if the username and password match
                if (strcmp(saved_username, username) == 0 && strcmp(saved_password, password) == 0) {
                    printf("Authentication successful\n");
                    //unlocking the file
                    lock.l_type=F_UNLCK;
                    if(fcntl(file,F_SETLK,&lock)==-1){
                        printf("Error unlocking Users file\n");
                        exit(EXIT_FAILURE);
                    }
                    close(file);
                    return 1; // User found
                }

                // Reset line buffer
                line_length = 0;
            } else {
                // Copy character to line buffer
                line[line_length] = buffer[i];
                line_length++;
            }
        }
    }
    //unlocking the file
    lock.l_type=F_UNLCK;
    if(fcntl(file,F_SETLK,&lock)==-1){
        printf("Error unlocking Users file\n");
        exit(EXIT_FAILURE);
    }
    // Close the file
    close(file);

    // Authentication failed
    printf("Authentication failed\n");
    return 0;
}
// int main(){
//     char name[]="zen";
//     char password[]="zendaya";
//     // printf("%d\n",isUserPresent(name));
//     // addLibrarian(name,password);
//     printf("%d\n",authenticateLibrarian(name,password));
//     return 0;
// }