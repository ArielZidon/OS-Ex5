#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    char value[1024];
    struct Node* next;
}Node;

void PUSH(Node** root, char value[1024]){
    Node* new_node = malloc(sizeof(Node));
    int i = 0;
    if(new_node ==  NULL){
        printf("Error to PUSH");
    }
    new_node->next = NULL;
    while(value[i] != '\0'){
        new_node->value[i] = value[i];
        i++;
    }
    Node* curr = *root;
    while(curr->next != NULL){
        curr = curr->next;
    }
    curr->next = new_node;
}

void TOP(Node** root){
    Node* curr = *root;
    while(curr->next != NULL){
        curr = curr->next;
    }
    printf("OUTPUT: %s\n", curr->value);
}

void POP(Node** root){
    if ((*root) == NULL){
        printf("ERROR: the stack is empty");
        exit(1);
    }
    Node* curr = *root;
    while(curr->next->next != NULL){
        curr = curr->next;
    }
    printf("%s\n", curr->next->value);
    curr->next = NULL;
}
void CLEAR(Node** root){
    (*root)->next = NULL;
    for(int i = 0; i < 1024; i++){
        (*root)->value[i] = '\0';
    }
}

int main(){
    Node* root = malloc(sizeof(Node));
    char str[1029] = {'\0'};
    char temp[1024] = {'\0'};
    scanf("%[^\n]%*c", str);
    while (strncmp(str, "EXIT", 4))
    {
        if(strncmp(str, "PUSH", 4) == 0){
            for(int i = 0; i < 1024 && str[i] != '\0'; i++){
                temp[i] = str[i+5];
            }
            PUSH(&root, temp);
        }
        else if(strncmp(str, "TOP", 3) == 0){
            TOP(&root);
        }
        else if(strncmp(str, "POP", 3) == 0){
            POP(&root);
        }
        else if(strncmp(str, "CLEAR", 5) == 0){
            CLEAR(&root);
        }

        scanf("%[^\n]%*c", str);
    }
    return 0;
}