#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#define RECORD_SIZE 900
void addLibrarian(char* username, char* password){
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
    printf("Librarian successfully added\n");
    close(file);
}
void addUser(char* username, char* password){
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
}
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

                // Extract username and password from the line
                char *saved_username = strtok(line, ":");
                char *saved_password = strtok(NULL, ":");

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
int main(){
    char name[]="ash turner";
    char password[]="ash turner is a good boy";
    // printf("%d\n",isUserPresent(name));
    // addUser(name,password);
    printf("%d\n",authenticateUser(name,password));
    return 0;
}