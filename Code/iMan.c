#include "../main.h"

void fetch_man_page(char *cmd) {
    char *temp;
    char *command_name;

    temp = malloc(strlen(cmd) + 1);
    if (temp == NULL) {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    strcpy(temp, cmd);

    command_name = strtok_r(temp, " \t\n", &temp);  

    while(command_name != NULL && (command_name[0] == '<' || command_name[0] == '>')) {
        trim_spaces(command_name);
        command_name = strtok_r(NULL, " \t\n", &temp);
        if(command_name == NULL) {
            perror("Invalid IO Redirection");
            return;
        } 

        command_name = strtok_r(NULL, " \t\n", &temp);
    }

    if(command_name == NULL) {
        perror("Invalid Command");
        return;
    }

    command_name = strtok_r(temp, " \t\n", &temp);
    if (command_name == NULL) {
        perror("Usage: iMan <command_name>\n");
        return;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        return;
    }

    struct hostent * server = gethostbyname("man.he.net");
    if (server == NULL) {
        perror("Error: No such host\n");
        close(sockfd);
        return;
    }

    struct in_addr **addr_list = (struct in_addr **)server->h_addr_list;
    if (addr_list[0] == NULL) {
        perror("No IP addresses found\n");
        close(sockfd);
        return;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr = *addr_list[0];

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        if (errno != EINPROGRESS) {
            perror("Error connecting");
            close(sockfd);
            return;
        }
    }

    char request[100001];
    snprintf(request, sizeof(request),
             "GET /?topic=%s&section=all HTTP/1.1\r\n"
             "Host: man.he.net\r\n"
             "Connection: close\r\n"
             "\r\n",
             command_name);

    if (write(sockfd, request, strlen(request)) < 0) {
        perror("Error writing to socket");
        close(sockfd);
        return;
    }

    bool header_done = false;
    size_t header_len = 0;
    while (1) {
        char buffer[100001];
        ssize_t  bytes_received = read(sockfd, buffer, sizeof(buffer) - 1);
        if (bytes_received < 0) {
            perror("Error reading from socket");
            break;
        } else if (bytes_received == 0) {
            break; 
        }

        header_len += bytes_received;
        buffer[header_len] = '\0';

        if (!header_done) {
            char *header_end = strstr(buffer, "\n\n");
            if (header_end) {
                header_done = true;
                printWithoutTags(header_end + 2);
                // printf("%s\n", header_end + 2);
            }
        } else {
            printWithoutTags(buffer);
            // printf("%s", buffer);
        }
    }

    printf("\n");
    close(sockfd);
    return;
}

void printWithoutTags(char *buffer) {
    bool dontPrint = false;
    for(int i = 0; i<strlen(buffer); i++) {
        if(buffer[i] == '<') {
            dontPrint = true;
        } else if (buffer[i] == '>') {
            dontPrint = false;
        } else if (!dontPrint) {
            printf("%c",buffer[i]);
        }
    }
    return;
}

