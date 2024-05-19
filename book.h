#ifndef BOOK_H
#define BOOK_H
void addBook(char* bookname,char* numCopies);
void issueBook(char* bookname,char* username);
void returnBook(char* bookname,char* username);
void deleteBook(char* bookname,char* username);
void updateName(char* oldName,char* newName);
void findAuthorName(char* bookname,char* authorName);
#endif