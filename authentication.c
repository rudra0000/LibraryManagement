#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
void addLibrarian(char* name){
    strcat(name,"\n");
    int file=open("librarians.txt",O_WRONLY | O_APPEND);
    if(file==-1){
        printf("Error opening librarians file\n");
        exit(1);
    }
    int sz=write(file,name,strlen(name));
    printf("Librarian successfully added\n");
    close(file);
}
void addUser(char *name){
    strcat(name,"\n");
    int file=open("users.txt",O_WRONLY | O_APPEND);
    if(file==-1){
        printf("Error opening users file\n");
        exit(1);
    }
    int sz=write(file,name,strlen(name));
    printf("User successfully added\n");
    close(file);
}
int isLibrarianPresent(char* name){
    int file;
    ssize_t bytes_read;
    char buffer[1000];
    char line[1000];
    int line_length = 0;

    file = open("librarians.txt", O_RDONLY);
    if (file == -1) {
        printf("Error opening file");
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
    close(file);
    return 0;
}
int isUserPresent(char* name){
    int file;
    ssize_t bytes_read;
    char buffer[1000];
    char line[1000];
    int line_length = 0;

    file = open("users.txt", O_RDONLY);
    if (file == -1) {
        printf("Error opening file");
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
    close(file);
    return 0;
}
int main(){
    char name[]="ketan";
    printf("%d\n",isUserPresent(name));
    return 0;
}