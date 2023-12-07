#include <stdio.h>
#include <winsock2.h>
#include <stdbool.h> //������ �������.
#include <pthread.h>
#include <string.h>
#include "stack.h"
#include "queue.h"
#include "hash.h"
#include "set.h"
bool usedSet = false;   //�������.
bool usedTable = false; //�������.
bool usedStack = false; //�������.
bool usedQueue = false; //�������.
char* fileName = NULL;  //�������.
char* query = NULL;     //�������.
char* op1 = NULL;       //�������.
char* op2 = NULL;       //�������.
//-lws2_32

void handleQuery(Set* set, Stack* stack, Queue* queue, HashTable* table, const char* query, const char* op1, const char* op2) {
    //������������ ������ ����� �� ���� ��������, �� ���������� ������ ��������� (��������� ��������� ������).
    if (!query || !op1 || !op2) {
        // ��������� ������: ������������ ������� ������
        printf("Error: Invalid input parameters\n");
        return;
    }
    if (strcmp(query, "SPUSH") == 0) { //STACK PUSH
        addStack(stack, op1);
        usedStack = true;
    }
    else if (strcmp(query, "SPOP") == 0) { //STACK POP & READ
        delStack(stack);
        printStack(stack);
        usedStack = true;
    }
    else if (strcmp(query, "SADD") == 0) { //SET ADD
        sadd(set, op1);
        usedSet = true;
    }
    else if (strcmp(query, "SREM") == 0) { //SET REM
        srem(set, op1);
        usedSet = true;
    }
    else if (strcmp(query, "SISMEMBER") == 0) { //SET CHECK FOR ACTUAL
        printf("%d\n", sismember(set, op1));
        usedSet = true;
    }
    else if (strcmp(query, "QPUSH") == 0) { //QUEUE PUSH
        qpush(queue, op1);
        usedQueue = true;
    }
    else if (strcmp(query, "QPOP") == 0) { //QUEUE POP, READ
        qpop(queue);
        printQueue(queue);
        usedQueue = true;
    }
    else if (strcmp(query, "HSET") == 0) { //HASH ADD
        printf("Adding: %s, %s\n", op1, op2);
        hset(table, op1, op2);
        usedTable = true;
    }
    else if (strcmp(query, "HDEL") == 0) { //HASH REMOVE
        hdel(table, op1);
        usedTable = true;
    }
    else if (strcmp(query, "HGET") == 0) { //HASH GET
        printf("%s\n", hget(table, op1));
        usedTable = true;
    }
    printf("Query: %s, Key: %s, Value: %s\n", query, op1, op2); //�����.

}
pthread_mutex_t mutex;
void* handle_client(void* arg) {
    SOCKET client_socket = (SOCKET)arg;

    if (client_socket == INVALID_SOCKET) {
        // ��������� ������: ������������ �����
        printf("Error: Invalid socket\n");
        return NULL;
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        printf("Error receiving data from client\n");
        closesocket(client_socket);
        return NULL;
    }
    else {
        printf("Received message: %s\n", buffer);

        char* token = strtok(buffer, " ");
        int arg_count = 0;

        char* fileToken = NULL;
        char* queryToken = NULL;
        char* op1Token = NULL;
        char* op2Token = NULL;

        while (token != NULL) {
            if (strcmp(token, "--file") == 0) {
                token = strtok(NULL, " ");
                fileToken = token;
                arg_count++;
            }
            else if (strcmp(token, "--query") == 0) {
                token = strtok(NULL, " ");
                queryToken = token;
                token = strtok(NULL, " ");
                op1Token = token;
                token = strtok(NULL, " ");
                op2Token = token;
                arg_count += 3; // ����������� ������� ���������� �� 3
            }
            token = strtok(NULL, " ");
        }

        if (arg_count < 2) { // ��������� �� ���������� ���������� ���������� (������ ���� 4: --file, --query, op1, op2)
            // ��������� ������: ������������ ���������� ����������
            printf("Error: Invalid number of arguments\n");
            closesocket(client_socket);
            return NULL;
        }

        if (!fileToken || !queryToken || !op1Token || !op2Token) {
            // ��������� ������: ���������� ������������ ����������
            printf("Error: Missing required arguments\n");
            closesocket(client_socket);
            return NULL;
        }

        fileName = fileToken;
        query = queryToken;
        op1 = op1Token;
        op2 = op2Token;

        Stack* stack = createStack();
        Queue* queue = createQueue();
        HashTable* table = new_table();
        Set* set = new_set();
        if (!stack || !queue || !table || !set) {
            // ��������� ������: �� ������� ������� ��������� ������
            printf("Error: Unable to create data structures\n");
            closesocket(client_socket);
            return NULL;
        }
        loadSetFromFile(set, fileName);      //������ ����������� ���� ���� ��� �������������, ��� � �������.
        loadTableFromFile(table, fileName);  //������ ����������� ���� ���� ��� �������������, ��� � �������.
        loadStackFromFile(stack, fileName);  //������ ����������� ���� ���� ��� �������������, ��� � �������.
        loadQueueFromFile(queue, fileName);  //������ ����������� ���� ���� ��� �������������, ��� � �������.
        pthread_mutex_lock(&mutex);
        handleQuery(set, stack, queue, table, query, op1, op2);
        pthread_mutex_unlock(&mutex);


        if (usedSet) {                              //����������, ���� ������� (������� � ������ ��� ����)).
            saveSetToFile(set, fileName);           //����������, ���� ������� (������� � ������ ��� ����)).
        }                                           //����������, ���� ������� (������� � ������ ��� ����)).
                                                    //����������, ���� ������� (������� � ������ ��� ����)).
        if (usedTable) {                            //����������, ���� ������� (������� � ������ ��� ����)).
            saveTableToFile(table, fileName);       //����������, ���� ������� (������� � ������ ��� ����)).
        }                                           //����������, ���� ������� (������� � ������ ��� ����)).
                                                    //����������, ���� ������� (������� � ������ ��� ����)).
        if (usedStack) {                            //����������, ���� ������� (������� � ������ ��� ����)).
            saveStackToFile(stack, fileName);       //����������, ���� ������� (������� � ������ ��� ����)).
        }                                           //����������, ���� ������� (������� � ������ ��� ����)).
                                                    //����������, ���� ������� (������� � ������ ��� ����)).
        if (usedQueue) {                            //����������, ���� ������� (������� � ������ ��� ����)).
            saveQueueToFile(queue, fileName);       //����������, ���� ������� (������� � ������ ��� ����)).
        }                                           //����������, ���� ������� (������� � ������ ��� ����)).
    


        // �������� ��������� ������� �� �������� ��������� ������
        int bytes_sent = send(client_socket, "Hello!!!", 8, 0);
        if (bytes_sent != strlen("Hello!!!")) {
            printf("Error sending data to client\n");
        }

        // �������� ����������� ��������� ������� �������
        int bytes_sentt = send(client_socket, buffer, bytes_received, 0);
        if (bytes_sentt != bytes_received) {
            printf("Error sending received data to client\n");
        }

        closesocket(client_socket);
    }

    return NULL;
}



int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6379);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);
    pthread_mutex_init(&mutex, NULL); // ������������� ��������

    while (1) {
        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);

        SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void*)client_socket);
        pthread_detach(thread); // ��������� ������

    }
    pthread_mutex_destroy(&mutex); // ������������ �������� ��������

    closesocket(server_socket);
    WSACleanup();

    return 0;
}
