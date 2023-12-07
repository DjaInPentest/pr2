#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 6379
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    memset(buffer, 0, sizeof(buffer));

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation error : %d", WSAGetLastError());
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Connection failed, retry... \n");
        main();
        return 1;
    }

    while (1) {
        printf("Usage: --file bible.data --query command op1, op2\nNow, print your command:\n");
        fgets(buffer, sizeof(buffer), stdin);

        if (send(clientSocket, buffer, strlen(buffer), 0) < 0) {
            printf("Send failed\n");
            main();
            return 1;
        }
        if (send(clientSocket, buffer, strlen(buffer), 0) < 0) {
            printf("Send failed\n");
            main();
            return 1;
        }

        memset(buffer, 0, sizeof(buffer));

        if (recv(clientSocket, buffer, BUFFER_SIZE, 0) < 0) {
            printf("Receive failed\n");
            main();
            return 1;
        }

        printf("%s\n", buffer);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
