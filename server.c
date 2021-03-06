/*
** server.c -- a stream socket server demo
*/
#include"header.h"
#include"stack.c"
#include <stdio.h>
// #include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>
#include<sys/mman.h>

#define PORT "3490"  // the port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold

Stack *s = NULL;



int openlock()
{
    fd = open(locking, O_WRONLY | O_CREAT);
    if (fd < 0)
    {
        perror("Error!!\nthe file has not opend");
    }
    memset(&lock, 0, sizeof(lock));
}

void* My_fork(int new_fd){
    int num;
    char str[1024];
    char temp[1024];

    while (1)
    {
        if ((num = recv(new_fd, str, 1023, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        str[num] = '\0';
        printf("client: received '%s'\n",str);

        if(strncmp(str, "EXIT", 4) == 0){
            printf("GOOD BYE!!\n\n");
            close(new_fd);
            return NULL;
        }
        else if(strncmp(str, "PUSH", 4) == 0){
            
            PUSH(s, &str[5]);
            printf("HELLO\n");
            bzero(str, 1024);
            strcpy(str, "make push");
        }
        else if(strncmp(str, "POP", 3) == 0){
            POP(s);
        }
        else if(strncmp(str, "TOP", 3) == 0){
            bzero(str,1024);
            strcpy(str, TOP(s));
        }
        else{
            printf("GOOD BYE\n\n");
            close(new_fd);
            exit(1);
        }
        printf("%s\n", str);
        if(send(new_fd, str, strlen(str), 0) == -1){
            printf("ERROR! from server");
            exit(1);
        } 
    }  
	close(new_fd);
    return NULL;
}

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;

}
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    openlock();
    s = mmap(NULL, sizeof(Stack), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); 
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    struct flock lock;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");
	int i = 0;
    while(1){  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
		if(!fork())
        {
            My_fork(new_fd);
            close(sockfd);
            exit(1);
        }
	}
    munmap(s, sizeof(Stack));
    return 0;
}