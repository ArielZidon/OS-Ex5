#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "memory.h"

#define NUMBER 1024*100

typedef struct Stack{
    int count;
    char value[NUMBER];
}Stack;

void PUSH(Stack* root, char* str){
    for(int i = 0; i < strlen(str); i++){
        root->value[root->count] = str[i];
        root->count++;
    }
    root->value[root->count++] = '\0';
}

char* TOP(Stack* root){
    int t = 0;

    char* res = (char*)malloc(sizeof(char)*1024);

    if(root->count == 0){
        printf("Error! stack is empty");
    }
    int x = (root->count)-2;
    while(root->value[x] != '\0'){
        x--;
    }
    for(int i = x; i <=root->count; i++){
        res[t]=root->value[i];
        t++;
    }
    return res;
}

void POP(Stack* root){
    while(root->value[root->count-2] != '\0'){
        root->count--;
    }
    root->count--;
}

void print_stack(Stack* root){
    for (int i = 0; i < root->count; i++)
    {
        printf("%c", root->value[i]);
        if(root->value[i] == '\0'){
            printf("\n");
        }
    } 
}

// int main(){
//     Stack* root = malloc(sizeof(Stack));
//     PUSH(root, "afik");
//     TOP(root);
//     PUSH(root, "ariel");
//     TOP(root);
//     PUSH(root, "hello");
//     TOP(root);
//     POP(root);
//     TOP(root);
//     // print_stack(root);
//     return 0;
// }