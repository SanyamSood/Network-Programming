#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 7007

int port = 0;
char *path;

void ServerSetup(char Http[]){
    FILE *filepointer;
    filepointer = fopen(strcat(path, "index.html"), "r");
    if (filepointer == NULL)
        exit(EXIT_FAILURE);
    char *line = NULL;
    size_t length = 0;
    ssize_t read;
    while ((read = getline(&line, &length, filepointer)) != -1) {
        strcat(Http, line);
    }
    fclose(filepointer);
}

void ConfiguringLoad(){
    FILE *filepointer;
    char *line = NULL;
    size_t length = 0;
    ssize_t read;
    filepointer = fopen("config.txt", "r");
    if (filepointer == NULL)
        exit(EXIT_FAILURE);
    while ((read = getline(&line, &length, filepointer)) != -1) {
        if(strstr(line, "PORT=") != NULL){
            port = atoi(line + 5);
        }
        if(strstr(line, "PATH=") != NULL){
            path = line + 5;
        }
    }
    fclose(filepointer);
    printf("File is loaded!! :)\n");
    printf("Port: %d\n", port);
    printf("Path: %s\n", path);
}

int main(int argc, char const *argv[]){
    int server_fd, new_socket; long valread;
    struct sockaddr_in servaddr;
    int addrlen = sizeof(servaddr);

    char Http[100000] = "HTTP/1.1 200 OK\r\n\n";
    ConfiguringLoad();
    ServerSetup(Http);
    printf("Header: %s\n", Http);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }


    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    memset(servaddr.sin_zero, '\0', sizeof servaddr.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&servaddr, sizeof(servaddr))<0)
    {
        perror("binding");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("listening");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n-----------------   Waiting for new connection Setup   -------------------------\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&servaddr, (socklen_t*)&addrlen))<0)
        {
            perror("accepting");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(new_socket , Http , strlen(Http));
        close(new_socket);
    }
    return 0;
}