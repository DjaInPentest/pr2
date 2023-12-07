#include <stdio.h>
#include <stdbool.h> //Только костыли.
#include <string.h> //strcmp.
#include "stack.h"
#include "queue.h"
#include "hash.h"
#include "set.h"
bool usedSet = false;   //Костыли.
bool usedTable = false; //Костыли.
bool usedStack = false; //Костыли.
bool usedQueue = false; //Костыли.
void handleQuery(Set* set, Stack* stack, Queue* queue, HashTable* table, const char* query, const char* op1, const char* op2) {
    //Обрабатываем запрос сразу из всех структур, но сохранится только выбранное (остальные стркутуры пустые).

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
    printf("Query: %s, Key: %s, Value: %s\n", query, op1, op2); //Дебаг.

}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printf("Usage: %s --file file.data --query command op1 op2(if needed)\n", argv[0]);
        return 1;
    }

    char* fileName = NULL; //Имя.
    char* query = NULL; //Запрос.
    char* op1 = NULL; //Первый операнд.
    char* op2 = NULL; //Второй операнд (да, назван неправильно)).
    // Обработка аргументов.
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) {
            fileName = argv[i + 1];
        }
        else if (strcmp(argv[i], "--query") == 0 && i + 1 < argc) {
            query = argv[i + 1];
            op1 = argv[i + 2];
            op2 = argv[i + 3];
        }
    }

    if (fileName == NULL || query == NULL) {
        printf("Usag1e: %s --file file.data --query 'SADD FIRST'\n", argv[0]);
        return 1;
    }
    Stack* stack = createStack();
    Queue* queue = createQueue();
    HashTable* table = new_table();
    Set* set = new_set();


    loadSetFromFile(set, fileName);      //Данные подгрузятся если файл уже использовался, уже с данными.
    loadTableFromFile(table, fileName);  //Данные подгрузятся если файл уже использовался, уже с данными.
    loadStackFromFile(stack, fileName);  //Данные подгрузятся если файл уже использовался, уже с данными.
    loadQueueFromFile(queue, fileName);  //Данные подгрузятся если файл уже использовался, уже с данными.
    printf("File: %s\n", fileName);
    printf("Query: %s\n", query);

    handleQuery(set, stack, queue, table, query, op1, op2);


    if (usedSet) {                              //Сохранится, если выбрано (костыли в начале для ифов)).
        saveSetToFile(set, fileName);           //Сохранится, если выбрано (костыли в начале для ифов)).
    }                                           //Сохранится, если выбрано (костыли в начале для ифов)).
                                                //Сохранится, если выбрано (костыли в начале для ифов)).
    if (usedTable) {                            //Сохранится, если выбрано (костыли в начале для ифов)).
        saveTableToFile(table, fileName);       //Сохранится, если выбрано (костыли в начале для ифов)).
    }                                           //Сохранится, если выбрано (костыли в начале для ифов)).
                                                //Сохранится, если выбрано (костыли в начале для ифов)).
    if (usedStack) {                            //Сохранится, если выбрано (костыли в начале для ифов)).
        saveStackToFile(stack, fileName);       //Сохранится, если выбрано (костыли в начале для ифов)).
    }                                           //Сохранится, если выбрано (костыли в начале для ифов)).
                                                //Сохранится, если выбрано (костыли в начале для ифов)).
    if (usedQueue) {                            //Сохранится, если выбрано (костыли в начале для ифов)).
        saveQueueToFile(queue, fileName);       //Сохранится, если выбрано (костыли в начале для ифов)).
    }                                           //Сохранится, если выбрано (костыли в начале для ифов)).
                                                //Сохранится, если выбрано (костыли в начале для ифов)).
    return 0;
}
